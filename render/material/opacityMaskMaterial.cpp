/*****************************************************************//**
 * \file   opacityMaskMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "opacityMaskMaterial.h"

OpacityMaskMaterial::OpacityMaskMaterial()
	: mDiffuse(nullptr)
	, mSpecularMask(nullptr)
	, mShiness(1.0f)
{
	mType = MaterialType::OpacityMaskMaterial;
}

OpacityMaskMaterial::~OpacityMaskMaterial() {

}