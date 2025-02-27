/*****************************************************************//**
 * \file   phongInstanceMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "phongInstanceMaterial.h"

PhongInstanceMaterial::PhongInstanceMaterial()
	: mDiffuse(nullptr)
	, mSpecularMask(nullptr)
	, mShiness(1.0f)
{
	mType = MaterialType::PhongInstanceMaterial;
}

PhongInstanceMaterial::~PhongInstanceMaterial() {

}