/*****************************************************************//**
 * \file   mesh.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "mesh.h"

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, bool IsShowAABB)
	: mGeometry(geometry)
	, mMaterial(material)
	, mIsShowAABB(IsShowAABB)
{
	mType = ObjectType::Mesh;
}

Mesh::~Mesh(){

}