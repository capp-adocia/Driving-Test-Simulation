﻿/*****************************************************************//**
 * \file   camera.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "camera.h"

//可以在这里把初始化参数都传进来
Camera::Camera()
	: mPosition(0.0f, 5.0f, 5.0f)
	, mFront(0.0f, 0.0f, -1.0f)
	, mUp(0.0f, 1.0f, 0.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mNear(0.1f)
	, mFar(1000.0f)
{}

Camera::~Camera() {}


glm::mat4 Camera::getViewMatrix() {
	//lookat 
	//-eye:相机位置（使用mPosition）
	//-center：看向世界坐标的哪个点
	//-top：穹顶（使用mUp替代）
	//glm::vec3 front = glm::cross(mUp, mRight);
	glm::vec3 center = mPosition + mFront;
	return glm::lookAt(mPosition, center, mUp);
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::identity<glm::mat4>();
}