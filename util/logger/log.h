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
public:
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;
public:
    void init(LogLevel level, uint8_t appenderTypes, const char* folder = "logs");
    // 设置日志级别
    void setLogLevel(LogLevel level);
    inline LogLevel getLogLevel() const { return m_currentLevel; }

    template <typename... Args>
    void log(LogLevel level, const std::string& format, const char* file, int line, Args&&... args) {
        if (!m_init) {
            printColor(LOG_LEVEL_WARN, "Logger is not initialized!");
            return;
        }
        if (level < m_currentLevel) return;
        std::string message = Util::formatMessage(format, std::forward<Args>(args)...);
        const std::string fileName = Util::getLastCppOrHFile(file);
        message += " [File: " + fileName + ", Line: " + std::to_string(line) + "]";
        // 输出日志
        outputLog(level, message);
    }

private:
    Logger();
private:
    LogLevel m_currentLevel = LOG_LEVEL_INFO;
    std::mutex m_mutex;
    std::vector<std::shared_ptr<Appender>> m_appenders{};
    bool m_init = false;
private:
    inline void addAppender(const std::shared_ptr<Appender> appender) { m_appenders.push_back(appender); }
    void outputLog(LogLevel level, const std::string& message);
};

/* 日志级别宏定义 */
#define LOG_DEBUG(format, ...) \
    Logger::getInstance().log(LOG_LEVEL_DEBUG, format, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO(format, ...)  \
    Logger::getInstance().log(LOG_LEVEL_INFO, format, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_WARN(format, ...)  \
    Logger::getInstance().log(LOG_LEVEL_WARN, format, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
    Logger::getInstance().log(LOG_LEVEL_ERROR, format, __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_INIT(level, appenderTypes, folder) \
    Logger::getInstance().init(level, appenderTypes, folder)

#endif // LOG_H
