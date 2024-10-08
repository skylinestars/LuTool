#ifndef LUTOOL_STRTOOL
#define LUTOOL_STRTOOL
#include <string>
#include <Common/Common.h>
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
	};

}

#endif // !LUTOOL_STRTOOL
