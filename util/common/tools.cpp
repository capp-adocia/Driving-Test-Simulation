/*****************************************************************//**
 * \file   tools.cpp
 * \brief  工具文件，提供一些常用的函数
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "tools.h"

namespace Util {
    BoundingSphere CalculateBoundingSphere(const std::vector<glm::vec3>& vertices) {
        if (vertices.empty()) return { glm::vec3(0), 0.0f };
        glm::vec3 minX = vertices[0], maxX = vertices[0];
        for (const auto& v : vertices) {
            if (v.x < minX.x) minX = v;
            if (v.x > maxX.x) maxX = v;
        }
        glm::vec3 center = (minX + maxX) * 0.5f;
        float radius = glm::length(maxX - center);
        for (const auto& v : vertices) {
            float dist = glm::length(v - center);
            if (dist > radius) {
                float newRadius = (radius + dist) * 0.5f;
                float delta = dist - radius;
                center = center + (v - center) * (delta / (2 * dist));
                radius = newRadius;
            }
        }
        return { center, radius };
    }
	std::vector<glm::vec3> GenerateSphereWireframe(
		const glm::vec3& center,
		float radius,
		int longitudeSegments,  // 经线分段数
		int latitudeSegments    // 纬线分段数
	) 
	{
		std::vector<glm::vec3> vertices;

		// 生成经线（纵向圆圈）
		for (int i = 0; i <= longitudeSegments; ++i) {
			float theta = glm::two_pi<float>() * i / longitudeSegments;
			for (int j = 0; j < latitudeSegments; ++j) {
				float phi = glm::pi<float>() * j / latitudeSegments;
				glm::vec3 pos = center + radius * glm::vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
				vertices.push_back(pos);
				phi = glm::pi<float>() * (j + 1) / latitudeSegments;
				pos = center + radius * glm::vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
				vertices.push_back(pos);
			}
		}

		// 生成纬线（横向圆圈）
		for (int j = 0; j < latitudeSegments; ++j) {
			float phi = glm::pi<float>() * j / latitudeSegments;
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);
			for (int i = 0; i <= longitudeSegments; ++i) {
				float theta = glm::two_pi<float>() * i / longitudeSegments;
				glm::vec3 pos = center + radius * glm::vec3(sinPhi * cos(theta), cosPhi, sinPhi * sin(theta));
				vertices.push_back(pos);
				theta = glm::two_pi<float>() * (i + 1) / longitudeSegments;
				pos = center + radius * glm::vec3(sinPhi * cos(theta), cosPhi, sinPhi * sin(theta));
				vertices.push_back(pos);
			}
		}

		return vertices;
	}
	bool SphereInFrustum(const glm::vec3& center, float radius, const Plane planes[6]) {
		for (int i = 0; i < 6; i++) {
			// normal先归一化再点乘中心点+距离
			float distance = glm::dot(planes[i].normal, center) + planes[i].distance;
			if (distance < -radius) return false; // 完全在平面外
		}
		return true;
	}
	Plane NormalizePlane(const glm::vec4& p) {
		float len = glm::length(glm::vec3(p)); // Compute length of the normal vector (ignoring w)
		return { glm::vec3(p) / len, p.w / len }; // Normalize and divide by the w component for distance
	}

	void ExtractPlanes(Plane planes[6], const glm::mat4& m) {
		// Accessing rows of the matrix manually by using the glm::mat4 indexing
		glm::mat4 transposed = glm::transpose(m);
		const glm::vec4 row0 = transposed[0];
		const glm::vec4 row1 = transposed[1];
		const glm::vec4 row2 = transposed[2];
		const glm::vec4 row3 = transposed[3];

		// Extract planes and normalize them
		planes[0] = NormalizePlane(row3 + row0); // Left
		planes[1] = NormalizePlane(row3 - row0); // Right
		planes[2] = NormalizePlane(row3 + row1); // Bottom
		planes[3] = NormalizePlane(row3 - row1); // Top
		planes[4] = NormalizePlane(row3 + row2); // Near
		planes[5] = NormalizePlane(row3 - row2); // Far
	}
}