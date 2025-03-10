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

 //========================================
 //         !平台无关通用配置
 //========================================

 /* 相机系统配置 */
 // 选择相机控制类型：游戏相机 / 载具相机
#define CameraControlType GameCameraControl  // 默认游戏相机控制
//#define CameraControlType VehicleCameraControl  // 载具相机控制（需配合物理系统使用）

/* SDL主循环处理 */
#define SDL_MAIN_HANDLED  // 禁用SDL默认主循环

/* 车辆手动挡和自动挡 */
//#define MANUALGEAR  // 手动挡
#define AUTOGEAR    // 自动挡

/* 单元测试 */
#define UNIT_T

//========================================
//         !平台相关配置
//========================================

#if defined(_WIN32) // !platform START
//-------------------------------
//       Windows平台配置
//-------------------------------
#include <windows.h>    // Windows核心头文件
#include <Psapi.h>      // 进程状态API
#include <direct.h>
#ifndef APIENTRY
#define APIENTRY __stdcall
#endif


// 调试模式配置
#ifdef DEBUG
//#define VLD_CHECK  // 启用内存泄漏检测

#else

#endif // DEBUG

//-------------------------------
#else  // 非Windows平台
//-------------------------------
//   Linux/macOS通用配置
#include <time.h>
#include <sys/resource.h>

// 调试模式配置
#ifdef DEBUG

#else



#endif

#endif // !platform END



#endif // PREPROCESSOR_H