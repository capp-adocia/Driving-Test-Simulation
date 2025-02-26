/*****************************************************************//**
 * \file   trackBallCameraControl.cpp
 * \brief  轨迹球相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "trackBallCameraControl.h"

TrackBallCameraControl::TrackBallCameraControl()
	: mMoveSpeed(0.005f)
{

}

TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::onCursor(double xpos, double ypos) {
	if (mLeftMouseDown) {
		//调整相机的各类参数
		//1 计算经线跟纬线旋转的增量角度(正负都有可能）
		float deltaX = static_cast<float>(xpos - mCurrentX) * mSensitivity;
		float deltaY = static_cast<float>(ypos - mCurrentY) * mSensitivity;

		//2 分开pitch跟yaw各自计算
		pitch(-deltaY);
		yaw(-deltaX);
	}
	else if (mMiddleMouseDown) {
		float deltaX = static_cast<float>(xpos - mCurrentX) * mMoveSpeed;
		float deltaY = static_cast<float>(ypos - mCurrentY) * mMoveSpeed;

		mCamera->mPosition += mCamera->mUp * deltaY;
		mCamera->mPosition -= mCamera->mRight * deltaX;
	}


	mCurrentX = static_cast<float>(xpos);
	mCurrentY = static_cast<float>(ypos);
}

void TrackBallCameraControl::pitch(float angle) {
	//绕着mRight向量在旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);

	//影响当前相机的up向量和位置 
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);//vec4给到vec3，给了xyz
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::yaw(float angle) {
	//绕着世界坐标系的y轴旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
	mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);
	mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::onScroll(float offset) {
	mCamera->scale(mScaleSpeed * offset);
}