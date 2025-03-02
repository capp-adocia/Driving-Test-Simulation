/*****************************************************************//**
 * \file   log.cpp
 * \brief  记录日志
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "log.h"
Logger::Logger()
    : m_mutex{}
    , m_currentLevel(LOG_LEVEL_INFO)
    , m_appenders({})
    , m_init(false)
{}

void Logger::init(LogLevel level, uint8_t appenderTypes, const char* folder) {
    m_init = true;
    setLogLevel(level);
    if (appenderTypes & AppenderType::APD_CONSOLE) {
        addAppender(std::make_shared<AppenderConsole>());
        printColor(LOG_LEVEL_INFO, "CONSOLE appender added.");
    }
    if (appenderTypes & AppenderType::APD_FILE) {
        addAppender(std::make_shared<AppenderFile>(folder));
        printColor(LOG_LEVEL_INFO, "FILE appender added.");
    }
    printColor(LOG_LEVEL_INFO, "Logger initialized!");
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (level >= LOG_LEVEL_DEBUG && level <= LOG_LEVEL_ERROR) {
        m_currentLevel = level;
        return;
    }
    printColor(LOG_LEVEL_WARN, "Invalid log level!");
}

void Logger::outputLog(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
    for (auto& appender : m_appenders) {
        appender->append(level, message);
    }
}