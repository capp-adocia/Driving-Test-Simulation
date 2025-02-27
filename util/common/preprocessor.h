/*****************************************************************//**
 * \file   preprocessor.h
 * \brief  预处理器宏定义，用于控制代码编译和调试
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#ifdef DEBUG
/* !DEBUG */
#define CameraControlType GameCameraControl
//#define CameraControlType vehicleCameraControl
#else
/* !RELEASE */

#endif // DEBUG

/* Platform */
#if defined(_WIN32)
#include <windows.h>
#include <Psapi.h>
#else
#include <time.h>
#include <sys/resource.h>
#endif


#endif // PREPROCESSOR_H
