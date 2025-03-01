/*****************************************************************//**
 * \file   vehicleCameraControl.cpp
 * \brief  车辆视角相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "vehicleCameraControl.h"
#include "../../util/logger/log.h"

VehicleCameraControl::VehicleCameraControl()
	: Position(glm::vec3(0.0f, 0.0f, 0.0f))
	, CameraTarget(glm::vec3(0.0f, 0.0f, 0.0f))
{}

VehicleCameraControl::~VehicleCameraControl() {}

void VehicleCameraControl::update()
{
	if ((Position == glm::vec3(0.0f, 0.0f, 0.0f)) || (CameraTarget == glm::vec3(0.0f, 0.0f, 0.0f))) {
		LOG_ERROR("Position or CameraTarget is not initialized!");
	}
	mCamera->mPosition = Position;
    mCamera->mFront = glm::normalize(CameraTarget - Position);
}
