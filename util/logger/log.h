/*****************************************************************//**
 * \file   log.h
 * \brief  记录日志
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef LOG_H
#define LOG_H

#include "logCommon.h"
#include "appenderConsole.h"
#include "appenderFile.h"
#include "../common/tools.h"

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // 初始化日志系统，设置日志级别和 Appender
    void init(LogLevel level, const std::shared_ptr<Appender> appender = nullptr) {
        setLogLevel(level);
        if(appender)
            addAppender(appender);
        else
            addAppender(std::make_shared<AppenderConsole>());
    }

    // 设置日志级别
    void setLogLevel(LogLevel level) { m_currentLevel = level; }
    LogLevel getLogLevel() const { return m_currentLevel; }

    // 设置日志输出目标（控制台和文件）
    void addAppender(const std::shared_ptr<Appender> appender) {
        m_appenders.push_back(appender);
    }

    // 日志记录接口
    template <typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) {
        if (level < m_currentLevel) return;

        std::string message = formatMessage(format, std::forward<Args>(args)...);
        outputLog(level, message);
    }

    // 禁止拷贝和移动
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

private:
    Logger() = default;
    LogLevel m_currentLevel = LOG_LEVEL_INFO;

    std::mutex m_mutex; // 确保线程安全
    std::vector<std::shared_ptr<Appender>> m_appenders;

    // 日志输出实现
    void outputLog(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
        // 输出到所有的 appender
        for (auto& appender : m_appenders) {
            appender->append(level, message);
        }
    }
};

/* 日志级别宏定义 */
#define LOG_DEBUG(format, ...) \
    Logger::getInstance().log(LOG_LEVEL_DEBUG, format, ##__VA_ARGS__)

#define LOG_INFO(format, ...)  \
    Logger::getInstance().log(LOG_LEVEL_INFO, format, ##__VA_ARGS__)

#define LOG_WARN(format, ...)  \
    Logger::getInstance().log(LOG_LEVEL_WARN, format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
    Logger::getInstance().log(LOG_LEVEL_ERROR, format, ##__VA_ARGS__)

#endif // LOG_H
