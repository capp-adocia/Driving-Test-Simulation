/*****************************************************************//**
 * \file   GrassInstanceMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "GrassInstanceMaterial.h"

GrassInstanceMaterial::GrassInstanceMaterial()
    : mDiffuse(nullptr)
    , mOpacityMask(nullptr)
    , mShiness(1.0f)
    , mUVScale(10.0f)
    , mlightIntensity(1.0f)
    , mWindScale(0.1f)
    , mWindDirection(1.0f, 0.0f, 0.0f)
    , mPhaseScale(1.0f)
    , mCloudMask(nullptr)
    , mCloudWhite(1.0f, 1.0f, 1.0f)
    , mCloudBlack(0.0f, 0.0f, 0.0f)
    , mCloudUVScale(2.0f)
    , mCloudSpeed(0.2f)
    , mCloudLerp(0.2f)
{
	mType = MaterialType::GrassInstanceMaterial;
}

GrassInstanceMaterial::~GrassInstanceMaterial() {

}