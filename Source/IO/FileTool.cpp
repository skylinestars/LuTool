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
#include "lz4.h"
#include <vector>
#include <string>
#include "Logger.h"

using namespace LuTool;
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
        LOG_ERROR("ERROR SaveFile:",fileName);
    }
}

void LuTool::FileTool::CompressToFile(const std::string& filename, const std::string& content)
{
    // 准备压缩
    const int src_size = static_cast<int>(content.size());
    const int max_dst_size = LZ4_compressBound(src_size);

    if (max_dst_size <= 0) 
    {
        LOG_ERROR("LZ4_compressBound failed");
        return;
    }

    // 分配压缩缓冲区
    std::vector<char> compressed_buffer(max_dst_size);

    // 执行压缩
    const int compressed_size = LZ4_compress_default(
        content.data(),
        compressed_buffer.data(),
        src_size,
        max_dst_size
    );

    if (compressed_size <= 0) 
    {
        LOG_ERROR("Compression failed");
        return;
    }

    // 打开文件准备写入
    std::ofstream out_file(filename, std::ios::binary);
    if (!out_file) 
    {
        LOG_ERROR("Cannot open file for writing:",filename);
        return;
    }

    // 先写入原始数据大小(用于解压时分配缓冲区)
    out_file.write(reinterpret_cast<const char*>(&src_size), sizeof(src_size));

    // 再写入压缩后数据大小
    out_file.write(reinterpret_cast<const char*>(&compressed_size), sizeof(compressed_size));

    // 最后写入压缩数据
    out_file.write(compressed_buffer.data(), compressed_size);

    if (!out_file) 
    {
        LOG_ERROR("Failed to write to file");
        return;
    }
    LOG_INFO("Compression successful. Original size: ", src_size, 
        ", Compressed size: ", compressed_size, ", Ratio: ", (float)compressed_size / src_size * 100);
}

std::string LuTool::FileTool::DecompressFromFile(const std::string& filename)
{
    // 打开文件
    std::ifstream in_file(filename, std::ios::binary);
    if (!in_file) 
    {
        LOG_ERROR("Cannot open file for reading:", filename);
        return "";
    }

    // 读取原始数据大小
    int original_size = 0;
    in_file.read(reinterpret_cast<char*>(&original_size), sizeof(original_size));

    // 读取压缩数据大小
    int compressed_size = 0;
    in_file.read(reinterpret_cast<char*>(&compressed_size), sizeof(compressed_size));

    // 读取压缩数据
    std::vector<char> compressed_buffer(compressed_size);
    in_file.read(compressed_buffer.data(), compressed_size);

    if (!in_file) 
    {
        LOG_ERROR("Failed to read from file");
        return "";
    }

    // 准备解压缓冲区
    std::vector<char> decompressed_buffer(original_size);

    // 执行解压
    const int decompressed_size = LZ4_decompress_safe(
        compressed_buffer.data(),
        decompressed_buffer.data(),
        compressed_size,
        original_size
    );

    if (decompressed_size < 0) 
    {
        LOG_ERROR("Decompression failed");
        return "";
    }

    if (decompressed_size != original_size) 
    {
        LOG_ERROR("Decompressed size doesn't match original size");
        return "";
    }

    return std::string(decompressed_buffer.data(), decompressed_size);
}
