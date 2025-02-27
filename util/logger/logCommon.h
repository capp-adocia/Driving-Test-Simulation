/*****************************************************************//**
 * \file   logCommon.h
 * \brief  ��־�����ļ�
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
  * ��־����˵����                                                                     *
  *   DEBUG - ������Ϣ������ֵ��ָ���ַ�����̸��٣�                                   *
  *   INFO  - ϵͳ״̬����ʼ���������ؼ�������¼                                       *
  *   WARN  - ���������⣨�ļ������ڡ�����ȱʡֵ����Դ�ͷž��棩                       *
  *   ERROR - ���ش��󣨿�ָ����ʡ����ݿ�����ʧ�ܡ��ؼ��쳣��                         *
 \************************************************************************************/

#define COLOR_DEFAULT "\033[0m"  // Ĭ����ɫ - ��
#define COLOR_DEBUG "\033[36m"  // ������־ - ��
#define COLOR_INFO "\033[32m"  // ��
#define COLOR_WARN "\033[93m"  // ����
#define COLOR_ERROR "\033[31m"  // ��

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