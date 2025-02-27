/*****************************************************************//**
 * \file   phongEnvMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "phongEnvMaterial.h"

PhongEnvMaterial::PhongEnvMaterial()
    : mDiffuse(nullptr)
    , mSpecularMask(nullptr)
    , mEnv(nullptr)
    , mShiness(1.0f)
{
	mType = MaterialType::PhongEnvMaterial;
}

PhongEnvMaterial::~PhongEnvMaterial() {

}