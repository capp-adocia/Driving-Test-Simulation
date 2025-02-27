/*****************************************************************//**
 * \file   screenMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "screenMaterial.h"

ScreenMaterial::ScreenMaterial()
	: mScreenTexture(nullptr)
{
	mType = MaterialType::ScreenMaterial;
}

ScreenMaterial::~ScreenMaterial() {

}