/*****************************************************************//**
 * \file   preprocessor.h
 * \brief  预处理器宏定义，用于控制代码编译
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#ifdef DEBUG
#define CameraControlType GameCameraControl
#else
#define CameraControlType vehicleCameraControl
#endif // DEBUG

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif




#endif // PREPROCESSOR_H
