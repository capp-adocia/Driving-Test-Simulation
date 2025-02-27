/*****************************************************************//**
 * \file   GrassInstanceMaterial.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef GRASSINSTANCEMATERIAL_H
#define GRASSINSTANCEMATERIAL_H

#include "material.h"
#include "../../resource/texture/texture.h"

class GrassInstanceMaterial : public Material
{
public:
	GrassInstanceMaterial();
	~GrassInstanceMaterial();
	inline std::shared_ptr<Texture> getDiffuse() const { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getOpacityMask() const { assert(mOpacityMask); return mOpacityMask; }
	inline float& getUVScale() { return mUVScale; } // imgui
	inline float& getLightIntensity() { return mlightIntensity; } // imgui
	inline float& getWindScale() { return mWindScale; } // imgui
    inline glm::vec3& getWindDirection() { return mWindDirection; } // imgui
	inline float& getPhaseScale() { return mPhaseScale; } // imgui
	inline std::shared_ptr<Texture> getCloudMask() const { assert(mCloudMask); return mCloudMask; }
	inline glm::vec3& getCloudWhite() { return mCloudWhite; } // imgui
	inline glm::vec3& getCloudBlack() { return mCloudBlack; } // imgui
	inline float& getCloudUVScale() { return mCloudUVScale; } // imgui
	inline float& getCloudSpeed() { return mCloudSpeed; } // imgui
	inline float& getCloudLerp() { return mCloudLerp; } // imgui
	inline void setDiffuse(std::shared_ptr<Texture> diffuse) { mDiffuse = diffuse; }
	inline void setOpacityMask(std::shared_ptr<Texture> opacityMask) { mOpacityMask = opacityMask; }
	inline void setCloudMask(std::shared_ptr<Texture> cloudMask) { mCloudMask = cloudMask; }
public:
	std::shared_ptr<Texture> mDiffuse;
	std::shared_ptr<Texture> mOpacityMask;
	float mShiness;
	float mUVScale; // 控制草地的uv采样
	float mlightIntensity; // 控制草地的光照强度

	float mWindScale; // 风的强度
	glm::vec3 mWindDirection; // 风的方向
	float mPhaseScale; // 控制相位

	// 云朵
	std::shared_ptr<Texture> mCloudMask;
	glm::vec3 mCloudWhite;
	glm::vec3 mCloudBlack;
	float mCloudUVScale; // 控制云朵的uv采样，控制云彩流动
	float mCloudSpeed; // 控制云朵的移动速度
	float mCloudLerp; // 控制云朵颜色占比
};
#endif // GRASSINSTANCEMATERIAL_H