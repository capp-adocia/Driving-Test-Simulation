/*****************************************************************//**
 * \file   phongMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material
{
public:
	PhongMaterial();
	~PhongMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }

public:
	std::shared_ptr<Texture> mDiffuse;
	std::shared_ptr<Texture> mSpecularMask;
	float mShiness;
};
#endif // !PHONG_MATERIAL_H