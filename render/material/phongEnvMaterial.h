/*****************************************************************//**
 * \file   phongEnvMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PHONG_ENV_MATERIAL_H
#define PHONG_ENV_MATERIAL_H

#include "material.h"
#include "../../resource/texture/texture.h"

class PhongEnvMaterial : public Material
{
public:
	PhongEnvMaterial();
	~PhongEnvMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }
	inline std::shared_ptr<Texture> getEnv() { assert(mEnv); return mEnv; }
	inline float getShiness() { return mShiness; }

public:
	std::shared_ptr<Texture> mDiffuse;
	std::shared_ptr<Texture> mSpecularMask;
	std::shared_ptr<Texture> mEnv;
	float mShiness;
};
#endif // PHONG_ENV_MATERIAL_H