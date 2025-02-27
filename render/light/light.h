/*****************************************************************//**
 * \file   light.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef LIGHT_H
#define LIGHT_H
#include "../core.h"

class Light {
public:
	Light();
	~Light();
	inline float& GetIntensity() { return mIntensity; }

public:
	glm::vec3	mColor;
	float		mSpecularIntensity;
	float 		mIntensity; // 光强
};
#endif // !LIGHT_H