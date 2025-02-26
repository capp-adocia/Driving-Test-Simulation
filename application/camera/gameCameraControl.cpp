/*****************************************************************//**
 * \file   gameCameraControl.cpp
 * \brief  游戏相机控制类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "gameCameraControl.h"

GameCameraControl::GameCameraControl()
	: mPitch(0.0f)
	, mSpeed(0.1f)
{}

GameCameraControl::~GameCameraControl() {

}

void GameCameraControl::onCursor(double xpos, double ypos) {
	float deltaX = static_cast<float>(xpos - mCurrentX) * mSensitivity;
	float deltaY = static_cast<float>(ypos - mCurrentY) * mSensitivity;
	
	if (mRightMouseDown) {
		pitch(-deltaY);
		yaw(-deltaX);
	}

	mCurrentX = static_cast<float>(xpos);
	mCurrentY = static_cast<float>(ypos);
}

void GameCameraControl::pitch(float angle) {
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f) {
		mPitch -= angle;
		return;
	}

//在gameCameraControl的情况下，pitch不会影响mPosition
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
}

void GameCameraControl::yaw(float angle) {
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
}


void GameCameraControl::update()
{
	//最终移动方向
	glm::vec3 direction(0.0f);

	auto front = glm::cross(mCamera->mUp, mCamera->mRight);
	auto right = mCamera->mRight;
	mCamera->mFront = front;
	if (mKeyMap[SDLK_w]) {
		direction += front;
	}

	if (mKeyMap[SDLK_s]) {
		direction -= front;
	}

	if (mKeyMap[SDLK_a]) {
		direction -= right;
	}

	if (mKeyMap[SDLK_d]) {
		direction += right;
	}

	//此时direction有可能不为1的长度，也有可能是0的长度
	if (glm::length(direction) != 0) {
		direction = glm::normalize(direction);
		mCamera->mPosition += direction * mSpeed;
	}
}