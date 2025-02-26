/*****************************************************************//**
 * \file   pointLight.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"
#include "../object.h"

class PointLight :public Light , public Object{
public:
	PointLight();
	~PointLight();

public:
	float mK2;
	float mK1;
	float mKc;
};
#endif // !POINTLIGHT_H