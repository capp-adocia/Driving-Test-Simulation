/*****************************************************************//**
 * \file   opacityMaskMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef OPACITY_MASK_MATERIAL_H
#define OPACITY_MASK_MATERIAL_H
#include "material.h"
#include "../../resource/texture/texture.h"

// 给材质上蒙版
class OpacityMaskMaterial : public Material
{
public:
	OpacityMaskMaterial();
	~OpacityMaskMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }

public:
	std::shared_ptr<Texture> mDiffuse;
	std::shared_ptr<Texture> mSpecularMask; // 透明度蒙版
	float mShiness;
};
#endif // !OPACITY_MASK_MATERIAL_H