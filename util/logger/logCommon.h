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
#define COLOR_WARN "\033[33m"  // ��
#define COLOR_ERROR "\033[31m"  // ��

enum LogLevel : uint8_t {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_LENGTH
};

enum AppenderType : uint8_t {
    APD_FILE = 0,
    APD_CONSOLE,
    APD_LENGTH
};

#endif // LOGCOMMON_H