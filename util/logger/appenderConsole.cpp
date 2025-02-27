/*****************************************************************//**
 * \file   appenderConsole.cpp
 * \brief  ����̨���
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "appenderConsole.h"

AppenderConsole::AppenderConsole() { m_type = AppenderType::APD_CONSOLE; }

void AppenderConsole::append(LogLevel level, const std::string& logMessage) {
    std::lock_guard<std::mutex> lock(m_mutex); // ȷ���̰߳�ȫ
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        std::cout << COLOR_DEBUG << "[DEBUG] " << COLOR_DEFAULT << logMessage << std::endl;
        break;
    case LOG_LEVEL_INFO:
        std::cout << COLOR_INFO << "[INFO] " << COLOR_DEFAULT << logMessage << std::endl;
        break;
    case LOG_LEVEL_WARN:
        std::cout << COLOR_WARN << "[WARN] " << COLOR_DEFAULT << logMessage << std::endl;
        break;
    case LOG_LEVEL_ERROR:
        std::cout << COLOR_ERROR << "[ERROR] " << COLOR_DEFAULT << logMessage << std::endl;
        break;
    default:
        std::cout << "[UNKNOWN] " << logMessage << std::endl;
        std::cerr << "Unknown log level" << std::endl;
        break;
    }
}