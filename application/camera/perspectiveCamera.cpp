/*****************************************************************//**
 * \file   perspectiveCamera.cpp
 * \brief  透视投影相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "perspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
	: mFovy(fovy)
	, mAspect(aspect)
{
	Camera::mNear = near;
	Camera::mFar = far;
}
PerspectiveCamera::~PerspectiveCamera() {

}

glm::mat4 PerspectiveCamera::getProjectionMatrix() {
	//注意：传入的是fovy的角度，需要转化为弧度
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale) {
	auto front = glm::cross(mUp, mRight);
	mPosition += (front * deltaScale);
}