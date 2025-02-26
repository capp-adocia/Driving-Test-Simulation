/*****************************************************************//**
 * \file   mesh.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef MESH_H
#define MESH_H
#include "../object.h"
#include "../geometry.h"
#include "../material/material.h"

class Mesh : public Object {
public:
	Mesh() = default;
	explicit Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
	~Mesh();
	inline std::shared_ptr<Geometry> getGeometry() { assert(mGeometry); return mGeometry; }
	inline std::shared_ptr<Material> getMaterial() { assert(mMaterial); return mMaterial; }
	inline void setGeometry(std::shared_ptr<Geometry> geometry) { mGeometry = geometry; }
	inline void setMaterial(std::shared_ptr<Material> material) { mMaterial = material; }

protected:
	std::shared_ptr<Geometry> mGeometry;
	std::shared_ptr<Material> mMaterial;
};
#endif // !MESH_H