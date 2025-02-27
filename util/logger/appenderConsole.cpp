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
    printColor(level, logMessage);
}