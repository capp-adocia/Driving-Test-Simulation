/*****************************************************************//**
 * \file   appenderConsole.h
 * \brief  控制台输出
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef APPENDER_CONSOLE_H
#define APPENDER_CONSOLE_H
#include "appender.h"
 // 控制台日志输出类
class AppenderConsole : public Appender {
public:
    AppenderConsole();
    void append(LogLevel level, const std::string& logMessage) override;
private:
    std::mutex m_mutex;
};
#endif // APPENDER_CONSOLE_H