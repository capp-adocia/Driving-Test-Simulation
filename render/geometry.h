/*****************************************************************//**
 * \file   geometry.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../../util/common/opengl.h"

class Geometry {
public:
	Geometry(); // 默认使用这个
	// 模型加载
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<float>& colors,
		const std::vector<unsigned int>& indices
	);
	~Geometry();

	static std::shared_ptr<Geometry> createBox(float size);
	static std::shared_ptr<Geometry> createBox(const float length, const float height, const float width);
	static std::shared_ptr<Geometry> createSphere(float radius);
	static std::shared_ptr<Geometry> createPlane(float width, float height);
	static std::shared_ptr<Geometry> createScreenPlane();
	static std::shared_ptr<Geometry> createCylinder(float radius, float height);
public:
	GLuint getVao() const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }
	inline const Util::BoundingVolume& getAABB() const { return AABB; }
public:
	Util::BoundingVolume AABB;
	//Util::BoundingSphere boundingSphere;
	//glm::vec3 boundingSphereCenter;  // 局部空间包围球中心
	//float boundingSphereRadius;      // 局部空间包围球半径

private:
	GLuint mVao;
	GLuint mPosVbo;
	GLuint mUvVbo;
	GLuint mNormalVbo;
	GLuint mEbo;
	GLuint mColorVbo;

	uint32_t mIndicesCount;
};
#endif // GEOMETRY_H