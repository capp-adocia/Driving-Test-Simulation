/*****************************************************************//**
 * \file   cameraControl.cpp
 * \brief  相机控制类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "cameraControl.h"

CameraControl::CameraControl()
	: mLeftMouseDown(false)
	, mRightMouseDown(false)
	, mMiddleMouseDown(false)
	, mCurrentX(0.0f)
	, mCurrentY(0.0f)
	, mSensitivity(0.1f)
	, mKeyMap({})
	, mCamera(nullptr)
	, mScaleSpeed(0.1f)
{}


CameraControl::~CameraControl() {}

void CameraControl::onMouse(int button, int action, double xpos, double ypos) {

	//1 判断当前的按键，是否按下
	bool pressed = (action == SDL_MOUSEBUTTONDOWN) ? true : false;
	//2 如果按下，记录当前按下的位置
	if (pressed) {
		mCurrentX = static_cast<float>(xpos);
		mCurrentY = static_cast<float>(ypos);
	}

	//3 根据按下的鼠标按键不同，更改按键状态
	switch (button) {
	case SDL_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case SDL_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case SDL_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}

void CameraControl::onCursor(double /*xpos*/, double /*ypos*/)
{

}

void CameraControl::onKey(int key, int action, int /*mods*/) {
	//过滤掉repeat的情况
	//if (action == GLFW_REPEAT) {
	//	return;
	//}

	//1 检测按下或者抬起，给到一个变量
	bool pressed = (action == SDL_KEYDOWN) ? true : false;
	//2 记录在keyMap
	mKeyMap[key] = pressed;
}


void CameraControl::onScroll(float /*offset*/) {
}