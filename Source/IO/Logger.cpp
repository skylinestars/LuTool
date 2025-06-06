// logger.cpp
#include "Logger.h"
using namespace LuTool;

Logger::Logger() 
    : logLevel_(LogLevel::LOG_INFO),
      running_(false),
      maxQueueSize_(50000) 
{
    startWorkerThread();
}

Logger& LuTool::Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::~Logger() {
    stopWorkerThread();
    
    if (fileStream_.is_open()) {
        fileStream_.close();
    }
}

void Logger::setOutputFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (fileStream_.is_open()) {
        fileStream_.close();
    }
    
    filename_ = filename;
    fileStream_.open(filename_, std::ios::app);
    
    if (!fileStream_.is_open()) {
        std::cerr << "Failed to open log file: " << filename_ << std::endl;
    }
}

void Logger::startWorkerThread() {
    if (running_) return;
    
    running_ = true;
    workerThread_ = std::thread([this]() {
        processLogQueue();
    });
}

void Logger::stopWorkerThread() {
    if (!running_) return;
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }
    
    cv_.notify_one();
    
    if (workerThread_.joinable()) {
        workerThread_.join();
    }
}

void Logger::enqueueLog(const std::string& message) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // 队列满时丢弃最旧的日志
    if (logQueue_.size() >= maxQueueSize_) {
        logQueue_.pop();
    }
    
    logQueue_.push(message);
    cv_.notify_one();
}

void Logger::processLogQueue() {
    while (running_) {
        std::string message;
        
        {
            std::unique_lock<std::mutex> lock(mutex_);
            
            cv_.wait(lock, [this] {
                return !running_ || !logQueue_.empty();
            });
            
            if (!running_ && logQueue_.empty()) {
                break;
            }
            
            message = std::move(logQueue_.front());
            logQueue_.pop();
        }
        
        writeToConsole(message);
        writeToFile(message);
    }
}

void Logger::formatHeader(std::stringstream& ss, LogLevel level) {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    
    char timeBuffer[26];
    ctime_s(timeBuffer, sizeof(timeBuffer), &now_c);
    timeBuffer[24] = '\0'; // 移除换行符
    
    const char* levelStr;
    switch (level) {
        case LogLevel::LOG_DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::LOG_INFO:  levelStr = "INFO "; break;
        case LogLevel::LOG_WARNING: levelStr = "WARN "; break;
        case LogLevel::LOG_ERROR: levelStr = "ERROR"; break;
        case LogLevel::LOG_FATAL: levelStr = "FATAL"; break;
        default: levelStr = "UNKNOWN"; break;
    }
    
    ss << "[" << timeBuffer << "][" << levelStr << "][" 
       << std::this_thread::get_id() << "] ";
}

void Logger::writeToConsole(const std::string& message) {
    std::cout << message;
}

void Logger::writeToFile(const std::string& message) {
    if (fileStream_.is_open()) 
    {
        fileStream_ << message;
        fileStream_.flush();
    }
}