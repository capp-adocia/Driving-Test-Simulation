/*****************************************************************//**
 * \file   directionalLight.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "light.h"

class DirectionalLight:public Light{
public:
	DirectionalLight();
	~DirectionalLight();

public:
	glm::vec3 mDirection;
};
#endif // !DIRECTIONALLIGHT_H