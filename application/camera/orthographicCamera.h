/*****************************************************************//**
 * \file   orthographicCamera.h
 * \brief  正交投影相机类
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H
#include "camera.h"

class OrthographicCamera : public Camera {
public:
	OrthographicCamera(float l, float r, float t, float b, float n, float f);
	~OrthographicCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float deltaScale) override;

private://一次性赋值之后，不需要再更改了
	float mLeft;
	float mRight;
	float mTop;
	float mBottom;
	float mScale;
};
#endif // ORTHOGRAPHICCAMERA_H