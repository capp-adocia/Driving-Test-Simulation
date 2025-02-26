/*****************************************************************//**
 * \file   spotLight.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "light.h"
#include "../object.h"

class SpotLight : public Light, public Object {
public:
	SpotLight();
	~SpotLight();

public:
	glm::vec3	mTargetDirection;
	float		mInnerAngle;
	float		mOuterAngle;
};
#endif // !SPOTLIGHT_H