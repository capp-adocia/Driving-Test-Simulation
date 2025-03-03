/*****************************************************************//**
 * \file   tools.cpp
 * \brief  
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

}