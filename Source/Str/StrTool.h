#ifndef LUTOOL_STRTOOL
#define LUTOOL_STRTOOL
#include <string>
#include <Common/Common.h>
#include <iostream>
#include <vector>
namespace LuTool
{
	class LUTOOL_EXPORTS StrTool
	{
	public:
		static std::string TryConvertToGb2312(std::string const& str);
		static bool IsTextUTF8(const char* str, uint64_t length);
		static std::string utf8_to_gb2312(std::string const& strUtf8);
		static bool ContainsChinese(const char* str);

		static void GetPathSuffix(const std::string& fileName, std::string& fileSuffix);
		static void GetPathFolder(const std::string& fileName, std::string& fileSuffix);
		static void GetPathName(const std::string& filePath, std::string& fileName);
		static void GetPathNameAndSuffix(const std::string& filePath, std::string& fileName);
		static void NormalPath(std::string& filePath);
		static std::vector<std::string> SplitString(const std::string& str, const std::string& delimiter);

		static std::string GetAuthorization(long useDays, std::string machCode="");
		static bool IsExpir(std::string& authorCode);
		static std::string Encryption(std::string& str);
		static std::string Decrypt(std::string& str);
		static std::string Base64Decode(std::string& str);
		static std::string Base64Encode(std::string& str);
		static std::string getCPUID();
	private:
		static std::string XorEncryptDecrypt(const std::string& input, char key);
		static std::string CaesarEncrypt(std::string text, int shift);
		static std::string CaesarDecrypt(std::string text, int shift);
	private:
		static const  std::string base64_chars;
	};

}

#endif // !LUTOOL_STRTOOL
