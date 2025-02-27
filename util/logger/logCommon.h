/*****************************************************************//**
 * \file   logCommon.h
 * \brief  日志公共文件
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef LOGCOMMON_H
#define LOGCOMMON_H

#include "../common/standard.h"
#include "../common/tools.h"

 /************************************************************************************\
  * 日志级别说明：                                                                     *
  *   DEBUG - 调试信息（变量值、指针地址、流程跟踪）                                   *
  *   INFO  - 系统状态、初始化参数、关键操作记录                                       *
  *   WARN  - 非致命问题（文件不存在、配置缺省值、资源释放警告）                       *
  *   ERROR - 严重错误（空指针访问、数据库连接失败、关键异常）                         *
 \************************************************************************************/

#define COLOR_DEFAULT "\033[0m"  // 默认颜色 - 白
#define COLOR_DEBUG "\033[36m"  // 调试日志 - 青
#define COLOR_INFO "\033[32m"  // 绿
#define COLOR_WARN "\033[93m"  // 亮黄
#define COLOR_ERROR "\033[31m"  // 红

constexpr char* DEBUG_S = "[DEBUG] ";
constexpr char* INFO_S = "[INFO] ";
constexpr char* WARN_S = "[WARN] ";
constexpr char* ERROR_S = "[ERROR] ";

enum LogLevel : uint8_t {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
};

enum AppenderType : uint8_t {
    APD_CONSOLE = 0x01,
    APD_FILE = 0x02,
};

static void printColor(LogLevel level, const std::string& msg)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:
        std::cout << COLOR_DEBUG << DEBUG_S << COLOR_DEFAULT << msg << std::endl;
        break;
    case LOG_LEVEL_INFO:
        std::cout << COLOR_INFO << INFO_S << COLOR_DEFAULT << msg << std::endl;
        break;
    case LOG_LEVEL_WARN:
        std::cout << COLOR_WARN << WARN_S << COLOR_DEFAULT << msg << std::endl;
        break;
    case LOG_LEVEL_ERROR:
        std::cout << COLOR_ERROR << ERROR_S << COLOR_DEFAULT << msg << std::endl;
        ASSERT(msg);
        break;
    default:
        std::cout << "[UNKNOWN] " << msg << std::endl;
        std::cerr << "Unknown log level" << std::endl;
        break;
    }
}

#endif // LOGCOMMON_H