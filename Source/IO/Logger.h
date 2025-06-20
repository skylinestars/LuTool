// logger.h
#ifndef LOGGER_H
#define LOGGER_H
#include "../Common/Common.h"
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <ctime>
#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <functional>
#if FMT_FOUND
#include <fmt/format.h>
#include <fmt/ostream.h>
#endif


// 如果没有fmt库，尝试使用C++20的std::format
#if !FMT_FOUND
    #if __cplusplus >= 202002L
        #include <format>
        #define HAS_STD_FORMAT 1
    #else
        #define HAS_STD_FORMAT 0
    #endif
#endif

// 在头文件中定义，确保所有编译单元使用同一个实例
// logger.h (Project B)
#ifdef _MSC_VER
#define LOGGING_API __declspec(selectany)
#else
#define LOGGING_API __attribute__((weak))
#endif

namespace LuTool{

enum class LUTOOL_EXPORTS LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
};

class LUTOOL_EXPORTS Logger {
public:
    static Logger& getInstance(); // 不要在头文件中内联实现

    ~Logger();
    
    void setLogLevel(LogLevel level) { logLevel_ = level; }
    void setOutputFile(const std::string& filename);
    void setMaxQueueSize(size_t size) { maxQueueSize_ = size; }
    
    // 同步日志接口 - 支持格式化字符串
    template<typename... Args>
    void syncLog(LogLevel level, const std::string& format, const Args&... args) {
        if (logLevel_ > level) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        std::string message;
        
        #if FMT_FOUND
            try {
                message = fmt::format(format, args...);
            } catch (const std::exception& e) {
                message = fmt::format("Format error: {} - Original format: {}", e.what(), format);
            }
        #elif HAS_STD_FORMAT
            try {
                message = std::format(format, args...);
            } catch (const std::exception& e) {
                message = std::format("Format error: {} - Original format: {}", e.what(), format);
            }
        #else
            // 回退方案：简单的字符串拼接
            message = format;
        #endif
        
        std::stringstream ss;
        formatHeader(ss, level);
        ss << message << std::endl;
        
        const std::string& formattedMessage = ss.str();
        writeToConsole(formattedMessage);
        writeToFile(formattedMessage);
    }

    // 异步日志接口 - 支持格式化字符串
    template<typename... Args>
    void log(LogLevel level, const std::string& format, const Args&... args) {
        if (logLevel_ > level) return;
        
        std::string message;
        
        #if FMT_FOUND
            try {
                message = fmt::format(format, args...);
            } catch (const std::exception& e) {
                message = fmt::format("Format error: {} - Original format: {}", e.what(), format);
            }
        #elif HAS_STD_FORMAT
            try {
                message = std::format(format, args...);
            } catch (const std::exception& e) {
                message = std::format("Format error: {} - Original format: {}", e.what(), format);
            }
        #else
            // 回退方案：简单的字符串拼接
            message = format;
        #endif
        
        std::stringstream ss;
        formatHeader(ss, level);
        ss << message << std::endl;
        
        enqueueLog(ss.str());
    }

    // 快捷宏定义 - 支持格式化字符串
    #define LOG_ASYN_DEBUG(format, ...) LuTool::Logger::getInstance().log(LuTool::LogLevel::LOG_DEBUG, format, ##__VA_ARGS__)
    #define LOG_ASYN_INFO(format, ...)  LuTool::Logger::getInstance().log(LuTool::LogLevel::LOG_INFO, format, ##__VA_ARGS__)
    #define LOG_ASYN_WARN(format, ...)  LuTool::Logger::getInstance().log(LuTool::LogLevel::LOG_WARNING, format, ##__VA_ARGS__)
    #define LOG_ASYN_ERROR(format, ...) LuTool::Logger::getInstance().log(LuTool::LogLevel::LOG_ERROR, format, ##__VA_ARGS__)
    #define LOG_ASYN_FATAL(format, ...) LuTool::Logger::getInstance().log(LuTool::LogLevel::LOG_FATAL, format, ##__VA_ARGS__)
    
    #define LOG_DEBUG(format, ...) LuTool::Logger::getInstance().syncLog(LuTool::LogLevel::LOG_DEBUG, format, ##__VA_ARGS__)
    #define LOG_INFO(format, ...)  LuTool::Logger::getInstance().syncLog(LuTool::LogLevel::LOG_INFO, format, ##__VA_ARGS__)
    #define LOG_WARN(format, ...)  LuTool::Logger::getInstance().syncLog(LuTool::LogLevel::LOG_WARNING, format, ##__VA_ARGS__)
    #define LOG_ERROR(format, ...) LuTool::Logger::getInstance().syncLog(LuTool::LogLevel::LOG_ERROR, format, ##__VA_ARGS__)
    #define LOG_FATAL(format, ...) LuTool::Logger::getInstance().syncLog(LuTool::LogLevel::LOG_FATAL, format, ##__VA_ARGS__)

private:
    Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void startWorkerThread();
    void stopWorkerThread();
    void enqueueLog(const std::string& message);
    void processLogQueue();
    
    void formatHeader(std::stringstream& ss, LogLevel level);
    void writeToConsole(const std::string& message);
    void writeToFile(const std::string& message);

    std::atomic<LogLevel> logLevel_;
    std::atomic<bool> running_;
    std::atomic<size_t> maxQueueSize_;
    
    std::ofstream fileStream_;
    std::string filename_;
    
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<std::string> logQueue_;
    std::thread workerThread_;
};

#endif // LOGGER_H

}