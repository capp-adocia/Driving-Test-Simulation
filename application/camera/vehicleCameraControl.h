/*****************************************************************//**
 * \file   vehicleCameraControl.h
 * \brief  车辆视角相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef VEHICLE_CAMERA_CONTROL_H
#define VEHICLE_CAMERA_CONTROL_H
#include "cameraControl.h"

class VehicleCameraControl : public CameraControl {
public:
	VehicleCameraControl();
	~VehicleCameraControl();
	void update() override;

	inline void setCameraPosTarget(const glm::vec3& pos, const glm::vec3& target)
	{
		Position = pos;
		CameraTarget = target;
	}

	glm::vec3 Position;
	glm::vec3 CameraTarget;
};
#endif // !VEHICLE_CAMERA_CONTROL_H