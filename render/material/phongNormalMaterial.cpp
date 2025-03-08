/*****************************************************************//**
 * \file   phongNormalMaterial.cpp
 * \brief  ·¨ÏßÌùÍ¼²ÄÖÊ
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "phongNormalMaterial.h"

PhongNormalMaterial::PhongNormalMaterial()
	: mDiffuse(nullptr)
	, mSpecularMask(nullptr)
	, mNormalMap(nullptr)
	, mShiness(1.0f)
{
	mType = MaterialType::PhongNormalMaterial;
}

PhongNormalMaterial::~PhongNormalMaterial() {

}