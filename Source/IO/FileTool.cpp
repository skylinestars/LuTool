#include <IO/FileTool.h>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#include <iconv.h>
#include <cstring>
#endif

bool LuTool::FileTool::CopyFile(const std::string& sourcePath, const std::string& destinationPath)
{
    std::ifstream sourceFile(sourcePath, std::ios::binary);
    std::ofstream destinationFile(destinationPath, std::ios::binary);
    if (!sourceFile || !destinationFile) 
    {
        return false;
    }
    destinationFile << sourceFile.rdbuf();
    return true;
}

bool LuTool::FileTool::FileExists(const std::string& fileName)
{
    std::ifstream file(fileName);
    bool exist = file.good();
    file.close();
    return exist;
}

bool LuTool::FileTool::FolderExists(const std::string& folderName)
{
    struct stat info;
    if (stat(folderName.c_str(), &info) != 0)
        return false;
    else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

bool LuTool::FileTool::CreateFileFolder(const std::string& fileName)
{
    int status;
#ifdef _WIN32
    status = mkdir(fileName.c_str());
#else
    status = mkdir(fileName.c_str(), 0755);
#endif
    return status == 0 ? true : false;
}

void LuTool::FileTool::SaveFile(const std::string& fileName, std::string& content)
{
    FILE* file = fopen(fileName.c_str(), "w");
    if (file != NULL)
    {
        fprintf(file, "%s", content.c_str());
        fclose(file);
    }
    else {
        std::cout << "ERROR SaveFile" << std::endl;
    }
}
