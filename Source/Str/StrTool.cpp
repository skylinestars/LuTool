#include <Str/StrTool.h>
#include <algorithm> 
#include <fstream>
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <intrin.h> 
#else
#include <sys/stat.h>
#include <iconv.h>
#include <cstring>
#include <cpuid.h>
#endif
#include<vector>
#include <codecvt>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <limits>
using namespace std;
using namespace std::chrono;

const std::string LuTool::StrTool::base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string LuTool::StrTool::TryConvertToGb2312(std::string const& str)
{
    if (IsTextUTF8(str.c_str(), str.size()))
        return utf8_to_gb2312(str);

    return str;
}

bool LuTool::StrTool::IsTextUTF8(const char* str, uint64_t length)
{
    int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr;
    bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
    for (int i = 0; i < length; ++i)
    {
        chr = *(str + i);
        if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
            bAllAscii = false;
        if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
        {
            if (chr >= 0x80)
            {
                if (chr >= 0xFC && chr <= 0xFD)
                    nBytes = 6;
                else if (chr >= 0xF8)
                    nBytes = 5;
                else if (chr >= 0xF0)
                    nBytes = 4;
                else if (chr >= 0xE0)
                    nBytes = 3;
                else if (chr >= 0xC0)
                    nBytes = 2;
                else
                    return false;

                nBytes--;
            }
        }
        else //多字节符的非首字节,应为 10xxxxxx
        {
            if ((chr & 0xC0) != 0x80)
                return false;

            nBytes--;
        }
    }
    if (nBytes > 0) //违返规则
        return false;
    if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
        return false;

    return true;
}

#ifdef _WIN32
std::string LuTool::StrTool::utf8_to_gb2312(std::string const& strUtf8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
    std::wstring wTemp = cutf8.from_bytes(strUtf8);
#ifdef _MSC_VER
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    const wchar_t* pwszNext = nullptr;
    char* pszNext = nullptr;
    mbstate_t state = {};

    std::vector<char> buff(wTemp.size() * 2);
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
        (loc).out(state,
            wTemp.data(), wTemp.data() + wTemp.size(), pwszNext,
            buff.data(), buff.data() + buff.size(), pszNext);

    if (std::codecvt_base::ok == res)
    {
        return std::string(buff.data(), pszNext);
    }
    return "";
}
#else
int charset_convert(const char* from_charset, const char* to_charset,
    char* in_buf, size_t in_left, char* out_buf, size_t out_left)
{
    iconv_t icd = (iconv_t)-1;
    size_t sRet = -1;
    char* pIn = in_buf;
    char* pOut = out_buf;
    size_t outLen = out_left;

    if (NULL == from_charset || NULL == to_charset || NULL == in_buf || 0 >= in_left || NULL == out_buf || 0 >= out_left)
    {
        return -1;
    }

    icd = iconv_open(to_charset, from_charset);
    if ((iconv_t)-1 == icd)
    {
        return -1;
    }

    sRet = iconv(icd, &pIn, &in_left, &pOut, &out_left);
    if ((size_t)-1 == sRet)
    {
        iconv_close(icd);
        return -1;
    }

    out_buf[outLen - out_left] = 0;
    iconv_close(icd);
    return (int)(outLen - out_left);
}
std::string pcc::FileTool::utf8_to_gb2312(std::string const& strUtf8)
{
    char* pIn = new char[strUtf8.length() + 1];
    std::strcpy(pIn, strUtf8.c_str());
    char* pOut = NULL;
    size_t outLen = 0;
    size_t inLen = 0;
    int iRet = -1;

    inLen = strUtf8.size();
    outLen = inLen * 3;
    pOut = (char*)malloc(sizeof(char) * outLen);
    if (NULL == pOut)
    {
        delete[] pIn;
        delete[] pOut;
        return "";
    }
    memset(pOut, 0, sizeof(char) * outLen);
    charset_convert("UTF-8", "GB2312", pIn, inLen, pOut, outLen);

    std::string result = pOut;
    delete[] pIn;
    delete[] pOut;
    return result;
}
#endif

bool LuTool::StrTool::ContainsChinese(const char* str)
{
    char c;
    while (1)
    {
        c = *str++;
        if (c == 0) break; //如果到字符串尾则说明该字符串没有中文字符
        if (c & 0x80) //如果字符高位为1且下一字符高位也是1则有中文字符
            if (*str & 0x80) return true;
    }
    return false;
}

void LuTool::StrTool::GetPathSuffix(const std::string& fileName, std::string& fileSuffix)
{
    if (fileName.size() < 1)
    {
        return;
    }
    std::string::size_type pos = fileName.rfind('.');
    std::string strExt = fileName.substr(pos == std::string::npos ? fileName.length() : pos + 1);
    std::transform(strExt.begin(), strExt.end(), strExt.begin(), ::tolower);
    fileSuffix.clear();
    fileSuffix.append(strExt);
}
void LuTool::StrTool::GetPathFolder(const std::string& filePath, std::string& fileName)
{
    std::string filePathTemp = filePath;
    NormalPath(filePathTemp);
    std::string::size_type rx = filePath.rfind('/');
    if (rx == string::npos)
    {
        return;
    }
    fileName.clear();
    fileName.append(filePath.substr(0, rx));
}
void LuTool::StrTool::GetPathName(const std::string& filePath, std::string& fileName)
{
    std::string filePathTemp = filePath;
    NormalPath(filePathTemp);
    std::string::size_type rx = filePath.rfind('/');
    std::string::size_type dian = filePath.rfind('.');
    if (rx == string::npos || dian == string::npos)
    {
        return;
    }
    fileName.clear();
    fileName.append(filePath.substr(rx + 1, dian - rx - 1));
}
void LuTool::StrTool::GetPathNameAndSuffix(const std::string& filePath, std::string& fileName)
{
    std::string filePathTemp = filePath;
    NormalPath(filePathTemp);
    std::string::size_type rx = filePath.rfind('/');
    if (rx == string::npos)
    {
        return;
    }
    fileName.clear();
    fileName.append(filePath.substr(rx + 1, filePath.size() - rx - 1));
}
void LuTool::StrTool::NormalPath(std::string& filePath)
{
    replace(filePath.begin(), filePath.end(), '\\', '/'); //替换'\'为'/
}

std::vector<std::string> LuTool::StrTool::SplitString(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

std::string LuTool::StrTool::GetAuthorization(long useDays, std::string machCode)
{
    /// 1、授权码拼凑：标志+是否有机器码+机器码（可能没有）+到期时间戳
    /// 2、将授权码字符串异或加密
    /// 3、将授权码字符串凯撒加密
    /// 4、将授权码字符串进行base64编码
    std::string authCode = "AMRT#";
    if (machCode.length() > 1)
    {
        authCode.append("Y#");
        authCode.append(machCode);
        authCode.append("#");
    }
    else
    {
        authCode.append("N#");
    }
    auto now = system_clock::now();
    auto future = now + hours(24 * useDays);
    time_t future_time = system_clock::to_time_t(future);
    authCode.append(std::to_string(future_time));//"1705816390"
    /*stringstream ss;
    ss << put_time(localtime(&future_time), "%Y-%m-%d");
    string future_date_str = ss.str();*/
    return Base64Encode(Encryption(authCode));
}

bool LuTool::StrTool::IsExpir(std::string& authorCode)
{
    std::string code = Decrypt(Base64Decode(authorCode));
    std::vector<std::string> tokens = SplitString(code, "#");
    std::string endTimeStr = "";
    if (tokens.size() < 3)
    {
        return true;
    }
    if (tokens[0] != "AMRT")
    {
        return true;
    }
    if (tokens[1] == "Y")
    {
        if (tokens.size() != 4)
        {
            return true;
        }
        //机器码判断
        std::string cpuid = getCPUID();
        if (tokens[2] != cpuid)
        {
            return true;
        }
        endTimeStr = tokens[3];
    }
    else
    {
        endTimeStr = tokens[2];
    }
    auto now = system_clock::to_time_t(system_clock::now());
    long end = std::stol(endTimeStr);
    if (now > end)
    {
        return true;
    }
    return false;
}

std::string LuTool::StrTool::Encryption(std::string& str)
{
    string temp = CaesarEncrypt(str, 4);
    return XorEncryptDecrypt(temp, 'L');
}

std::string LuTool::StrTool::Decrypt(std::string& str)
{
    string temp = XorEncryptDecrypt(str, 'L');
    return CaesarDecrypt(temp, 4);
}

bool is_base64(unsigned char c) 
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string LuTool::StrTool::Base64Decode(std::string& encoded_string)
{
    size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

std::string LuTool::StrTool::Base64Encode(std::string& input)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t in_len = input.size();
    const char* bytes_to_encode = input.data();

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string LuTool::StrTool::getCPUID()
{
    std::stringstream ss;

#ifdef _WIN32
    // Windows平台使用__cpuid intrinsic
    int cpuInfo[4] = { 0 };
    __cpuid(cpuInfo, 0);

    char vendor[13] = { 0 };
    *reinterpret_cast<int*>(vendor) = cpuInfo[1];
    *reinterpret_cast<int*>(vendor + 4) = cpuInfo[3];
    *reinterpret_cast<int*>(vendor + 8) = cpuInfo[2];

    ss << "Vendor: " << vendor;

    // 获取处理器信息（如果可用）
    if (cpuInfo[0] >= 1) {
        __cpuid(cpuInfo, 1);
        ss << ", Processor: " << std::hex
            << (cpuInfo[3] >> 24) << (cpuInfo[3] >> 16 & 0xFF)
            << (cpuInfo[3] >> 8 & 0xFF) << (cpuInfo[3] & 0xFF);
    }
#else
    // Linux平台使用__get_cpuid
    unsigned int eax, ebx, ecx, edx;
    __get_cpuid(0, &eax, &ebx, &ecx, &edx);

    char vendor[13] = { 0 };
    memcpy(vendor, &ebx, 4);
    memcpy(vendor + 4, &edx, 4);
    memcpy(vendor + 8, &ecx, 4);

    ss << "Vendor: " << vendor;

    if (__get_cpuid_max(0, nullptr) >= 1) {
        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        ss << ", Processor: " << std::hex << eax;
    }
#endif

    return Base64Encode(ss.str());
}

std::string LuTool::StrTool::XorEncryptDecrypt(const std::string& input, char key)
{
    std::string output = input;
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key;
    }
    return output;
}


std::string LuTool::StrTool::CaesarEncrypt(string text, int shift)
{
    for (char& c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = base + (c - base + shift) % 26;
        }
    }
    return text;
}

std::string LuTool::StrTool::CaesarDecrypt(std::string text, int shift)
{
    return CaesarEncrypt(text, 26 - (shift % 26)); // 利用模运算特性解密
}
