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
	bool isGetMachine = false;
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
		if (value == "-getMachine")
		{
			std::string code = StrTool::getCPUID();
			LOG_INFO("machine code:", code);
			return 0;
		}
	}
	
	std::string authorCode = StrTool::GetAuthorization(day, machine);
	FileTool::CompressToFile("x.bin", authorCode);

	std::string fileData = FileTool::DecompressFromFile("x.bin");
	bool status = StrTool::IsExpir(fileData);
	LOG_INFO("status:", status);
	return 0;
}