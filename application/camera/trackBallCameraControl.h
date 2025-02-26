/*****************************************************************//**
 * \file   trackBallCameraControl.h
 * \brief  轨迹球相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef TRACKBALLCAMERACONTROL_H
#define TRACKBALLCAMERACONTROL_H

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl {
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	//父类当中的接口函数，是否需要重写
	void onCursor(double xpos, double ypos)override;

	void onScroll(float offset)override;

private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float mMoveSpeed;
};
#endif // !TRACKBALLCAMERACONTROL_H