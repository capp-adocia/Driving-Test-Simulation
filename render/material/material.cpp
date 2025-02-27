/*****************************************************************//**
 * \file   material.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "material.h"
Material::Material()
	: mType(MaterialType::PhongMaterial)
	, mDepthTest(true)
	, mDepthFunc(GL_LEQUAL)
	, mDepthWrite(true)
	, mPolygonOffset(false)
	, mPolygonOffsetType(GL_POLYGON_OFFSET_FILL)
	, mFactor(0.0f)
	, mUnit(0.0f)
	, mStencilTest(false)
	, mSFail(GL_KEEP)
	, mZFail(GL_KEEP)
	, mZPass(GL_KEEP)
	, mStencilMask(0xFF)
	, mStencilFunc(GL_ALWAYS)
	, mStencilRef(0)
	, mStencilFuncMask(0xFF)
	, mBlend(false)
	, mSFactor(GL_SRC_ALPHA)
	, mDFactor(GL_ONE_MINUS_SRC_ALPHA)
	, mOpacity(1.0f)
	, mFaceCulling(false)
	, mFrontFace(GL_CCW)
	, mCullFace(GL_BACK)
{}
Material::~Material() {}