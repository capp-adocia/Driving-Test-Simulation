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
	Geometry();
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

	GLuint getVao() const { return mVao; }
	uint32_t getIndicesCount() const { return mIndicesCount; }

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