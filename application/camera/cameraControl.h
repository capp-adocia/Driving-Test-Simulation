/*****************************************************************//**
 * \file   cameraControl.h
 * \brief  相机控制类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H
#include "camera.h"

class CameraControl {
public:
	CameraControl();
	virtual ~CameraControl();

	//由于继承CameraControl的子类，有可能会实现自己的逻辑
	virtual void onMouse(int button, int action, double xpos, double ypos);
	virtual void onCursor(double xpos, double ypos);
	virtual void onKey(int key, int action, int mods);
	virtual void onScroll(float offset);//+1 -1

	//每一帧渲染之前都要进行调用，每一帧更新的行为可以放在这里
	virtual void update() = 0;

	void setCamera(std::shared_ptr<Camera> camera) { mCamera = camera; }
	void setSensitivity(float s) { mSensitivity = s; }
	void setScaleSpeed(float s) { mScaleSpeed = s; }

protected:
	//1 鼠标按键状态
	bool mLeftMouseDown;
	bool mRightMouseDown;
	bool mMiddleMouseDown;

	//2 当前鼠标的位置
	float mCurrentX, mCurrentY;

	//3 敏感度
	float mSensitivity;

	//4 记录键盘相关按键的按下状态
	std::unordered_map<int, bool> mKeyMap;

	//5 存储当前控制的那一个摄像机
	std::shared_ptr<Camera> mCamera;

	//6 记录相机缩放的速度
	float mScaleSpeed;
};
#endif // !CAMERA_CONTROL_H