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

int main(int argc, char **argv)
{

	// 设置日志文件
	Logger::getInstance().setOutputFile("Authorization.log");

	// 设置日志级别
	Logger::getInstance().setLogLevel(LogLevel::LOG_INFO);
	bool isEnabled = true;
	bool isVisible = false;
	bool hasError = true;


	bool status = FileTool::CreateFolderRecursive("D:/test/test/test");
	LOG_INFO("status:{}", status);



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


	// 使用新的格式化字符串方式
	int edgeCount = 100;
	int faceCount = 50;
	int shellCount = 10;

	// 同步日志示例
	LOG_INFO("实体内容统计 - 边: {}, 面: {}, 壳体: {}", edgeCount, faceCount, shellCount);
	LOG_DEBUG("调试信息 - 当前处理进度: {:.2f}%", 75.5);
	LOG_WARN("警告信息 - 发现 {} 个重复项", 5);
	LOG_ERROR("错误信息 - 文件 {} 不存在", "config.xml");

	// 异步日志示例
	LOG_ASYN_INFO("异步日志 - 用户 {} 登录成功", "admin");
	LOG_ASYN_DEBUG("异步调试 - 内存使用: {} MB", 128.5);
	LOG_ASYN_WARN("异步警告 - 连接超时，重试第 {} 次", 3);
	LOG_ASYN_ERROR("异步错误 - 数据库连接失败: {}", "Connection refused");

	// 复杂格式化示例
	std::string fileName = "data.txt";
	double fileSize = 1024.5;
	LOG_INFO("文件信息 - 名称: {}, 大小: {:.1f} KB, 状态: {}",
			 fileName, fileSize, "已加载");

	// 等待异步日志处理完成
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "日志测试完成，请查看 Authorization.log 文件" << std::endl;
	return 0;
}