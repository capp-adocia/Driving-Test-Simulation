/*****************************************************************//**
 * \file   spotLight.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "spotLight.h"

SpotLight::SpotLight()
	: mTargetDirection(glm::vec3(-1.0f))
	, mInnerAngle(0.0f)
	, mOuterAngle(0.0f)
{

}

SpotLight::~SpotLight() {

}
