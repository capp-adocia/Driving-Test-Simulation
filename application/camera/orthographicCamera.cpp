/*****************************************************************//**
 * \file   orthographicCamera.cpp
 * \brief  正交投影相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "orthographicCamera.h"

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
	: mLeft(l)
	, mRight(r)
	, mTop(t)
	, mBottom(b)
	, mScale(1.0f)
{
	Camera::mNear = n;
	Camera::mFar = f;
}
OrthographicCamera::~OrthographicCamera() {

}

glm::mat4 OrthographicCamera::getProjectionMatrix(){
	float scale = std::pow(2.0f, mScale);
	return glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
}

void OrthographicCamera::scale(float deltaScale) {
	mScale += deltaScale;
}
