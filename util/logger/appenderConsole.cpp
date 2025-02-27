/*****************************************************************//**
 * \file   appenderConsole.cpp
 * \brief  控制台输出
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "appenderConsole.h"

AppenderConsole::AppenderConsole() { m_type = AppenderType::APD_CONSOLE; }

void AppenderConsole::append(LogLevel level, const std::string& logMessage) {
    std::lock_guard<std::mutex> lock(m_mutex); // 确保线程安全
    printColor(level, logMessage);
}