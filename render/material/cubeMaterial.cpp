/*****************************************************************//**
 * \file   cubeMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "cubeMaterial.h"

CubeMaterial::CubeMaterial()
	: mDiffuse(nullptr)
{
	mType = MaterialType::CubeMaterial;
}

CubeMaterial::~CubeMaterial() {

}