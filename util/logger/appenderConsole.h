/*****************************************************************//**
 * \file   appenderConsole.h
 * \brief  ����̨���
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef APPENDER_CONSOLE_H
#define APPENDER_CONSOLE_H
#include "appender.h"
 // ����̨��־�����
class AppenderConsole : public Appender {
public:
    AppenderConsole();
    void append(LogLevel level, const std::string& logMessage) override;
private:
    std::mutex m_mutex;
};
#endif // APPENDER_CONSOLE_H