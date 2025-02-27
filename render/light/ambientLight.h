/*****************************************************************//**
 * \file   ambientLight.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H
#include "light.h"

class AmbientLight :public Light {
public:
	AmbientLight();
	~AmbientLight();
};
#endif // !AMBIENTLIGHT_H