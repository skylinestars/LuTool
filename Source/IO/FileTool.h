#ifndef LUTOOL_FILETOOL
#define LUTOOL_FILETOOL

#include <string>
#include "../Common/Common.h"
namespace LuTool
{
	class LUTOOL_EXPORTS FileTool
	{
	public:
		static bool CopyFile(const std::string &sourcePath, const std::string &destinationPath);
		static bool FileExists(const std::string &fileName);
		static bool FolderExists(const std::string &folderName);
		static bool CreateFileFolder(const std::string &fileName);
		static bool CreateFolderRecursive(const std::string &folderName);
		static void SaveFile(const std::string &fileName, std::string &context);
	};
}
#endif // !LUTOOL_FILETOOL
