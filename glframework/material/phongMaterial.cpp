/*****************************************************************//**
 * \file   phongMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "phongMaterial.h"

PhongMaterial::PhongMaterial()
	: mDiffuse(nullptr)
	, mSpecularMask(nullptr)
	, mShiness(1.0f)
{
	mType = MaterialType::PhongMaterial;
}

PhongMaterial::~PhongMaterial() {

}