/*****************************************************************//**
 * \file   camera.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "../../util/common/opengl.h"

class Camera {
public:
	Camera();
	virtual ~Camera();

	glm::mat4 getViewMatrix();
	virtual glm::mat4 getProjectionMatrix();
	virtual void scale(float deltaScale) = 0;

public:
	glm::vec3 mPosition;
	glm::vec3 mUp;
	glm::vec3 mFront;
	glm::vec3 mRight;
	float mNear;
	float mFar;
};
#endif // CAMERA_H