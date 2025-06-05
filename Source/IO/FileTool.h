#ifndef LUTOOL_FILETOOL
#define LUTOOL_FILETOOL

#include <string>
#include <Common/Common.h>
namespace LuTool
{
	class LUTOOL_EXPORTS FileTool
	{
	public:
		static bool CopyFile(const std::string& sourcePath, const std::string& destinationPath);
		static bool FileExists(const std::string& fileName);
		static bool FolderExists(const std::string& folderName);
		static bool CreateFileFolder(const std::string& fileName);
		static void SaveFile(const std::string& fileName, std::string& context);
		static void CompressToFile(const std::string& filename, const std::string& content);
		static std::string DecompressFromFile(const std::string& filename);
	};
}
#endif // !LUTOOL_FILETOOL
