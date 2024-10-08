#include <Str/StrTool.h>
#include <algorithm> 
#include <fstream>
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <iconv.h>
#include <cstring>
#endif
#include<vector>
#include <codecvt>

using namespace std;
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