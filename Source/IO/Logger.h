// logger.h
#ifndef LOGGER_H
#define LOGGER_H
#include "Common/Common.h"
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
    
    // 同步日志接口
    template<typename... Args>
    void syncLog(LogLevel level, const Args&... args) {
        if (logLevel_ > level) return;
        
        std::lock_guard<std::mutex> lock(mutex_);
        std::stringstream ss;
        formatHeader(ss, level);
        (ss << ... << args);
        ss << std::endl;
        
        const std::string& message = ss.str();
        writeToConsole(message);
        writeToFile(message);
    }

    // 异步日志接口
    template<typename... Args>
    void log(LogLevel level, const Args&... args) {
        if (logLevel_ > level) return;
        
        std::stringstream ss;
        formatHeader(ss, level);
        (ss << ... << args);
        ss << std::endl;
        
        enqueueLog(ss.str());
    }

    // 快捷宏定义
    #define LOG_ASYN_DEBUG(...) Logger::getInstance().log(LogLevel::LOG_DEBUG, __VA_ARGS__)
    #define LOG_ASYN_INFO(...)  Logger::getInstance().log(LogLevel::LOG_INFO, __VA_ARGS__)
    #define LOG_ASYN_WARN(...)  Logger::getInstance().log(LogLevel::LOG_WARNING, __VA_ARGS__)
    #define LOG_ASYN_ERROR(...) Logger::getInstance().log(LogLevel::LOG_ERROR, __VA_ARGS__)
    #define LOG_ASYN_FATAL(...) Logger::getInstance().log(LogLevel::LOG_FATAL, __VA_ARGS__)
    
    #define LOG_DEBUG(...) Logger::getInstance().syncLog(LogLevel::LOG_DEBUG, __VA_ARGS__)
    #define LOG_INFO(...)  Logger::getInstance().syncLog(LogLevel::LOG_INFO, __VA_ARGS__)
    #define LOG_WARN(...)  Logger::getInstance().syncLog(LogLevel::LOG_WARNING, __VA_ARGS__)
    #define LOG_ERROR(...) Logger::getInstance().syncLog(LogLevel::LOG_ERROR, __VA_ARGS__)
    #define LOG_FATAL(...) Logger::getInstance().syncLog(LogLevel::LOG_FATAL, __VA_ARGS__)

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