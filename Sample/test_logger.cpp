#include "IO/Logger.h"
#include <iostream>

int main()
{
    std::cout << "测试新的Logger格式化功能..." << std::endl;

    // 设置日志文件
    LuTool::Logger::getInstance().setOutputFile("test_format.log");

    // 设置日志级别
    LuTool::Logger::getInstance().setLogLevel(LuTool::LogLevel::LOG_DEBUG);

    // 测试基本格式化
    int edgeCount = 100;
    int faceCount = 50;
    int shellCount = 10;

    LOG_INFO("实体内容统计 - 边: {}, 面: {}, 壳体: {}", edgeCount, faceCount, shellCount);
    LOG_DEBUG("调试信息 - 当前处理进度: {:.2f}%", 75.5);
    LOG_WARN("警告信息 - 发现 {} 个重复项", 5);
    LOG_ERROR("错误信息 - 文件 {} 不存在", "config.xml");

    // 测试bool类型 - 现在应该输出true/false而不是1/0
    bool isEnabled = true;
    bool isVisible = false;
    bool hasError = true;

    LOG_INFO("功能状态 - 启用: {}, 可见: {}, 错误: {}", isEnabled, isVisible, hasError);
    LOG_DEBUG("Bool测试 - 启用状态: {}, 可见状态: {}", isEnabled, isVisible);
    LOG_WARN("Bool警告 - 错误状态: {}", hasError);

    // 测试bool与其他类型混合
    int count = 5;
    std::string name = "test";
    LOG_DEBUG("混合类型测试 - 名称: {}, 数量: {}, 启用: {}", name, count, isEnabled);

    // 测试异步日志
    LOG_ASYN_INFO("异步日志 - 用户 {} 登录成功", "admin");
    LOG_ASYN_DEBUG("异步调试 - 内存使用: {} MB", 128.5);
    LOG_ASYN_INFO("异步bool测试 - 状态: {}, 标志: {}", isEnabled, isVisible);

    // 测试复杂格式化
    std::string fileName = "data.txt";
    double fileSize = 1024.5;
    LOG_INFO("文件信息 - 名称: {}, 大小: {:.1f} KB, 状态: {}",
             fileName, fileSize, "已加载");

    // 等待异步日志处理完成
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "测试完成！请查看 test_format.log 文件" << std::endl;
    return 0;
}