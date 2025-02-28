/*****************************************************************//**
 * \file   opengl.cpp
 * \brief  OpenGL相关头文件
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "opengl.h"
#include "../logger/log.h"

void checkError()
{
	GLenum errorCode = glGetError();
	std::string error = "";
	if (errorCode != GL_NO_ERROR) {
		switch (errorCode)
		{
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:  error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY: error = "OUT OF MEMORY"; break;
		default:error = "UNKNOWN"; break;
		}
		LOG_ERROR(error);
	}
}
