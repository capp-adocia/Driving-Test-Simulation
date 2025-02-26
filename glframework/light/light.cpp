/*****************************************************************//**
 * \file   light.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "light.h"

Light::Light()
	: mColor(glm::vec3(1.0f))
	, mSpecularIntensity(1.0f)
	, mIntensity(1.0f)
{

}

Light::~Light() {

}