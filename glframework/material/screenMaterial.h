/*****************************************************************//**
 * \file   screenMaterial.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef SCREENMATERIAL_H
#define SCREENMATERIAL_H
#include "material.h"
#include "../texture.h"

class ScreenMaterial : public Material {
public:
	ScreenMaterial();
	~ScreenMaterial();
	inline void setScreenTexture(std::shared_ptr<Texture> texture) { mScreenTexture = texture; }
	inline std::shared_ptr<Texture> getScreenTexture() { assert(mScreenTexture); return mScreenTexture; }
private:
	std::shared_ptr<Texture> mScreenTexture;
};
#endif // SCREENMATERIAL_H