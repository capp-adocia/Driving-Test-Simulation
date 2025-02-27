/*****************************************************************//**
 * \file   material.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef MATERIAL_H
#define MATERIAL_H
#include "../core.h"

//使用C++的枚举类型
enum class MaterialType {
	PhongMaterial,
	WhiteMaterial,
	DepthMaterial,
	OpacityMaskMaterial,
	ScreenMaterial,
	CubeMaterial, // 蒙鼓人
	PhongEnvMaterial, // 环境贴图
	PhongInstanceMaterial, // 实例绘制
	GrassInstanceMaterial, // 草地实例绘制
};

class Material {
public:
	Material();
	virtual ~Material();
	inline MaterialType getType() const { return mType; }
	// 深度检测
	inline bool getDepthTest() const { return mDepthTest; }
	inline GLenum getDepthFunc() const { return mDepthFunc; }
	inline bool getDepthWrite() const { return mDepthWrite; }
	inline void setType(MaterialType type) { mType = type; }
	inline void setDepthTest(bool depthTest) { mDepthTest = depthTest; }
	inline void setDepthFunc(GLenum depthFunc) { mDepthFunc = depthFunc; }
	inline void setDepthWrite(bool depthWrite) { mDepthWrite = depthWrite; }
	// polygon offset
	inline void setPolygonOffsetType(unsigned int polygonOffsetType) { mPolygonOffsetType = polygonOffsetType; }
	inline void setFactor(float factor) { mFactor = factor; }
	inline void setUnit(float unit) { mUnit = unit; }
	inline bool getPolygonOffset() const { return mPolygonOffset; }
	inline unsigned int getPolygonOffsetType() const { return mPolygonOffsetType; }
	inline float getFactor() const { return mFactor; }
	inline float getUnit() const { return mUnit; }
    // 模板测试
	inline bool getStencilTest() const { return mStencilTest; }
	inline unsigned int getSFail() const { return mSFail; }
	inline unsigned int getZFail() const { return mZFail; }
	inline unsigned int getZPass() const { return mZPass; }
	inline unsigned int getStencilMask() const { return mStencilMask; }
	inline unsigned int getStencilFunc() const { return mStencilFunc; }
	inline unsigned int getStencilRef() const { return mStencilRef; }
	inline unsigned int getStencilFuncMask() const { return mStencilFuncMask; }
	inline void setStencilTest(bool stencilTest) { mStencilTest = stencilTest; }
	inline void setSFail(unsigned int sFail) { mSFail = sFail; }
	inline void setZFail(unsigned int zFail) { mZFail = zFail; }
	inline void setZPass(unsigned int zPass) { mZPass = zPass; }
	inline void setStencilMask(unsigned int stencilMask) { mStencilMask = stencilMask; }
	inline void setStencilFunc(unsigned int stencilFunc) { mStencilFunc = stencilFunc; }
	inline void setStencilRef(unsigned int stencilRef) { mStencilRef = stencilRef; }
	inline void setStencilFuncMask(unsigned int stencilFuncMask) { mStencilFuncMask = stencilFuncMask; }
	// 颜色混合
	inline bool getBlend() const { return mBlend; }
	inline unsigned int getSFactor() const { return mSFactor; }
	inline unsigned int getDFactor() const { return mDFactor; }
	inline void setBlend(bool blend) { mBlend = blend; }
	inline void setSFactor(unsigned int sFactor) { mSFactor = sFactor; }
	inline void setDFactor(unsigned int dFactor) { mDFactor = dFactor; }
	// 总体透明度
	inline float getOpacity() const { return mOpacity; }
	inline void setOpacity(float opacity) { mOpacity = opacity; }
	// 面剔除
	inline bool getFaceCulling() const { return mFaceCulling; }
	inline unsigned int getFrontFace() const { return mFrontFace; }
	inline unsigned int getCullFace() const { return mCullFace; }
	inline void setFaceCulling(bool cullFace) { mFaceCulling = cullFace; }
	inline void setFrontFace(unsigned int frontFace) { mFrontFace = frontFace; }
	inline void setCullFace(unsigned int cullFace) { mCullFace = cullFace; }
	inline void setPolygonOffset(bool polygonOffset) { mPolygonOffset = polygonOffset; }
	

protected:
	MaterialType mType;
	// 深度检测
	bool mDepthTest;
	GLenum mDepthFunc; // 当前深度缓冲区值与输入深度值比较时小于等于时通过深度测试
	bool mDepthWrite;
	//polygonOffset相关
	bool			mPolygonOffset;
	unsigned int	mPolygonOffsetType;
	float			mFactor;
	float			mUnit;
	// 模板测试
	bool mStencilTest;
	unsigned int mSFail; // 模板测试失败时采取的操作
	unsigned int mZFail; // 模板测试通过，但深度测试失败时采取的操作
	unsigned int mZPass; // 模板测试通过，且深度测试通过时采取的操作
	unsigned int mStencilMask; // 模板缓冲区写入掩码
	unsigned int mStencilFunc; // 模板测试函数
	unsigned int mStencilRef; // 模板测试参考值
	unsigned int mStencilFuncMask; // 模板测试掩码

	// 颜色混合
	bool mBlend;
	unsigned int mSFactor; // 源颜色因子
    unsigned int mDFactor; // 目标颜色因子

	// 总体透明度
	float mOpacity;

	// 面剔除
	bool mFaceCulling;
	unsigned int mFrontFace;
	unsigned int mCullFace;
};
#endif // !MATERIAL_H