/*****************************************************************//**
 * \file   phongInstanceMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PHONGINSTANCEMATERIAL_H
#define PHONGINSTANCEMATERIAL_H

#include "material.h"
#include "../../resource/texture/texture.h"

class PhongInstanceMaterial : public Material
{
public:
	PhongInstanceMaterial();
	~PhongInstanceMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }
	inline float getShiness() { return mShiness; }
	inline void setDiffuse(std::shared_ptr<Texture> diffuse) { mDiffuse = diffuse; }
	inline void setSpecularMask(std::shared_ptr<Texture> specularMask) { mSpecularMask = specularMask; }

private:
	std::shared_ptr<Texture> mDiffuse;
	std::shared_ptr<Texture> mSpecularMask;
	float mShiness;
};
#endif // PHONGINSTANCEMATERIAL_H