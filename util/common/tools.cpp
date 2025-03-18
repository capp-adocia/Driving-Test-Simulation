/*****************************************************************//**
 * \file   tools.cpp
 * \brief  工具文件，提供一些常用的函数
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "tools.h"
#include "../logger/log.h"

namespace Util {

	BoundingVolume CalculateBoundingVolume(
		const std::vector<glm::vec3>& vertices,
		BoundingType type
	)
	{
		if (vertices.empty()) {
			switch (type)
			{
			case Util::BoundingType::Sphere: return BoundingSphere{ glm::vec3(0), 0.0f };
			case Util::BoundingType::AABB: return BoundingAABB{ glm::vec3(0), glm::vec3(0) };
			default: throw std::invalid_argument("Unsupported bounding type");
			}
		}

		switch (type) {
		case BoundingType::Sphere:
		{
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
			return BoundingSphere{ center, radius };
		}
		case BoundingType::AABB:
		{
			glm::vec3 min = vertices[0];
			glm::vec3 max = vertices[0];
			for (const auto& v : vertices) {
				min.x = std::min(min.x, v.x);
				min.y = std::min(min.y, v.y);
				min.z = std::min(min.z, v.z);
				max.x = std::max(max.x, v.x);
				max.y = std::max(max.y, v.y);
				max.z = std::max(max.z, v.z);
			}
			glm::vec3 center = (min + max) * 0.5f;
			glm::vec3 extents = max - center;
			return BoundingAABB{ center, extents };
		}
		default:
			throw std::invalid_argument("Unsupported bounding type");
		}
	}

	BoundingVolume TransformAABB(const BoundingVolume& localAABB, const glm::mat4& modelMatrix)
	{
		if (auto* aabb = std::get_if<BoundingAABB>(&localAABB))
		{
			// 计算局部空间AABB的8个角点
			const glm::vec3 min = aabb->center - aabb->extents;
			const glm::vec3 max = aabb->center + aabb->extents;

			const std::array<glm::vec3, 8> localCorners = {
				glm::vec3(min.x, min.y, min.z),
				glm::vec3(max.x, min.y, min.z),
				glm::vec3(max.x, max.y, min.z),
				glm::vec3(min.x, max.y, min.z),
				glm::vec3(min.x, min.y, max.z),
				glm::vec3(max.x, min.y, max.z),
				glm::vec3(max.x, max.y, max.z),
				glm::vec3(min.x, max.y, max.z)
			};

			// 初始化世界空间中的极值
			glm::vec3 worldMin(std::numeric_limits<float>::max());
			glm::vec3 worldMax(std::numeric_limits<float>::lowest());

			// 变换每个角点并更新极值
			for (const auto& corner : localCorners) {
				const glm::vec4 worldPos = modelMatrix * glm::vec4(corner, 1.0f);
				const glm::vec3 worldCorner(worldPos);

				worldMin = glm::min(worldMin, worldCorner);
				worldMax = glm::max(worldMax, worldCorner);
			}

			// 计算新的AABB
			const glm::vec3 newCenter = (worldMin + worldMax) * 0.5f;
			const glm::vec3 newExtents = worldMax - newCenter;

			return BoundingAABB{ newCenter, newExtents };
		}
		else if (auto aabb = std::get_if<BoundingSphere>(&localAABB)) {
			const glm::vec4 worldCenter = modelMatrix * glm::vec4(aabb->center, 1.0f);
			const float worldRadius = aabb->radius * glm::max(glm::max(glm::abs(modelMatrix[0][0]), glm::abs(modelMatrix[1][0])), glm::abs(modelMatrix[2][0]));
			return BoundingSphere{ glm::vec3(worldCenter), worldRadius };
		}
		LOG_WARN("BoundingType is not AABB");
		return localAABB;
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