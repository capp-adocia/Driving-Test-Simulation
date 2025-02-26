/*****************************************************************//**
 * \file   gameCameraControl.h
 * \brief  游戏相机控制类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef GAME_CAMERA_CONTROL_H
#define GAME_CAMERA_CONTROL_H

#include "cameraControl.h"

class GameCameraControl : public CameraControl {
public:
	GameCameraControl();
	~GameCameraControl();

	void onCursor(double xpos, double ypos) override; 
	void update()override;

	void setSpeed(float s) { mSpeed = s; }
private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float mPitch;
	float mSpeed;
};
#endif // GAME_CAMERA_CONTROL_H