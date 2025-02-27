/*****************************************************************//**
 * \file   appender.h
 * \brief  日志输出类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef APPENDER_H
#define APPENDER_H

#include "../common/standard.h"
#include "logCommon.h"
#include "../common/tools.h"

class Appender {
public:
    Appender() : m_type(AppenderType::APD_CONSOLE) {}
    virtual ~Appender() = default;
    virtual void append(LogLevel level, const std::string& logMessage) = 0;
    inline AppenderType getType() const {
        return m_type;
    }
protected:
    AppenderType m_type;
};

#endif // APPENDER_H