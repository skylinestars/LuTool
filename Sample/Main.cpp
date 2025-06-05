/**
 * 测试案例
 * Author: WangLu
 * Date:   2024-05-16
 */

#include <iostream>
#include "Str/StrTool.h"
#include "IO/FileTool.h"
#include "IO/Logger.h"
using namespace LuTool;

int main(int argc, char** argv)
{
	Logger::getInstance().setOutputFile("Authorization.log");
	LOG_INFO("Log: 授权码程序日志");
	long day = 0;
	std::string machine = "";
	for (unsigned int i = 1; i < argc; ++i)
	{
		std::string value = argv[i];
		if (value == "-d")
		{
			++i;
			day = std::stol(argv[i]);
		}
		if (value == "-m")
		{
			++i;
			machine = argv[i];
		}
	}
	std::string authorCode = StrTool::GetAuthorization(day, machine);
	std::cout << authorCode << std::endl;

	FileTool::CompressToFile("x.bin", authorCode);

	std::string fileData = FileTool::DecompressFromFile("x.bin");
	std::cout << fileData << std::endl;
	bool status = StrTool::IsExpir(authorCode);
	std::cout << status << std::endl;
	return 0;
}