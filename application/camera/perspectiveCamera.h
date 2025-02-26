/*****************************************************************//**
 * \file   perspectiveCamera.h
 * \brief  透视投影相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"
#include "../../util/common/opengl.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	~PerspectiveCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float deltaScale) override;

private:
	float mFovy = 0.0f;
	float mAspect = 0.0f;
};
#endif // PERSPECTIVECAMERA_H