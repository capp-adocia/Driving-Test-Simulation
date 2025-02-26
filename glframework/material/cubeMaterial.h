/*****************************************************************//**
 * \file   cubeMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef CUBEMATERIAL_H
#define CUBEMATERIAL_H

#include "material.h"
#include "../texture.h"

class CubeMaterial : public Material {
public:
	CubeMaterial();
	~CubeMaterial();
	inline void setDiffuse(std::shared_ptr<Texture> diffuse) { mDiffuse = diffuse; }
	inline std::shared_ptr<Texture> getDiffuse() { return mDiffuse; }
private:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
};
#endif // !CUBEMATERIAL_H