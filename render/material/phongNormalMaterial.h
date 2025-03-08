/*****************************************************************//**
 * \file   phongNormalMaterial.h
 * \brief  ·¨ÏßÌùÍ¼²ÄÖÊ
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "material.h"
#include "../../resource/texture/texture.h"

class PhongNormalMaterial : public Material {
public:
	PhongNormalMaterial();
	~PhongNormalMaterial();
public:
	std::shared_ptr<Texture>	mDiffuse;
	std::shared_ptr<Texture>	mSpecularMask;
	std::shared_ptr<Texture>	mNormalMap;
	float						mShiness;
};