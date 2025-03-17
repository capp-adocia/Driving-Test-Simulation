/*****************************************************************//**
 * \file   tools.h
 * \brief  工具文件，提供一些常用的函数
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef TOOLS_H
#define TOOLS_H
#include "standard.h"
#include "glm.h"

/* 宏工具 */
#define CONCAT(a, b) a##b

/* 调试日志输出宏 */
#define CERR std::cerr

/* 断言宏 配合log使用 */
#define ASSERT(x) do{\
        CERR << " Assertion "<< #x << " failed at " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::abort(); \
    } while(0);

namespace Util {
    /* 字符串格式化实现 V1 */
    inline std::string formatMessage(const std::string& format) {
        return format;
    }
    /* 字符串格式化实现 V2 */
    template <typename T, typename... Args>
    std::string formatMessage(const std::string& format, T&& first, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos == std::string::npos) {
            return format;
        }
        std::ostringstream stream;
        stream << format.substr(0, pos) << std::forward<T>(first);
        return stream.str() + formatMessage(format.substr(pos + 2), std::forward<Args>(args)...);
    }

    /* 获取最后一个cpp或h文件 */
    static std::string getLastCppOrHFile(const std::string& path) {
        size_t lastSlash = path.find_last_of("/\\");
        std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != std::string::npos) {
            std::string ext = fileName.substr(dotPos);
            if (ext == ".cpp" || ext == ".h") {
                return fileName;
            }
        }
        return "";
    }

    template<typename T>
    T* ptr(const T& ref)
    {
        return reinterpret_cast<T*>(&ref);
    }
    template<typename T, typename V>
    T& cast(V& ptr)
    {
        return *reinterpret_cast<T*>(&ptr);
    }
    template<typename T, typename V>
    const T& cast(const V& ptr)
    {
        return *reinterpret_cast<T*>((void*)&ptr);
    }

    struct BoundingSphere {
        glm::vec3 center;
        float radius;
    };
    struct BoundingAABB {
        glm::vec3 center;
        glm::vec3 extents; // 半长
    };

    using BoundingVolume = std::variant<BoundingSphere, BoundingAABB>;

    enum class BoundingType {
        Sphere,
        AABB
    };
    BoundingVolume CalculateBoundingVolume(
        const std::vector<glm::vec3>& vertices,
        BoundingType type = BoundingType::Sphere
    );

    BoundingVolume TransformAABB(const BoundingVolume& localAABB, const glm::mat4& modelMatrix);

    //BoundingSphere CalculateBoundingSphere(const std::vector<glm::vec3>& vertices);
    //std::vector<glm::vec3> GenerateSphereWireframe(const BoundingVolume& volume); // 生成球体线框
    
    //BoundingAABB CalculateAABB(const std::vector<glm::vec3>& vertices); // 计算AABB
    //std::vector<glm::vec3> GenerateAABBWireframe(const BoundingVolume& volume);

    // Ax + By + Cz + D = 0
    // A, B, C是法向量，D是距离原点的距离
    struct Plane {
        glm::vec3 normal;
        float distance;
    };

    struct FrustumCullVisitor {
        const Plane* planes;

        bool operator()(const BoundingSphere& sphere) {
            for (int i = 0; i < 6; ++i) {
                float distance = glm::dot(planes[i].normal, sphere.center) + planes[i].distance;
                if (distance < -sphere.radius) return false;
            }
            return true;
        }

        bool operator()(const BoundingAABB& aabb) {
            for (int i = 0; i < 6; ++i) {
                const float r = aabb.extents.x * std::abs(planes[i].normal.x)
                    + aabb.extents.y * std::abs(planes[i].normal.y)
                    + aabb.extents.z * std::abs(planes[i].normal.z);
                const float d = glm::dot(planes[i].normal, aabb.center) + planes[i].distance;
                if (d < -r) return false;
            }
            return true;
        }
    };

    static inline bool FrustumCullVariant(const BoundingVolume& volume, const Plane planes[6]) {
        return std::visit(FrustumCullVisitor{ planes }, volume);
    }
    struct WireframeGenerator {
        int longitudeSegments = 8;
        int latitudeSegments = 8;

        std::vector<glm::vec3> operator()(const BoundingSphere& sphere) {
            std::vector<glm::vec3> vertices;
            const glm::vec3& center = sphere.center;
            const float radius = sphere.radius;

            for (int i = 0; i <= longitudeSegments; ++i) {
                float theta = glm::two_pi<float>() * i / longitudeSegments;
                for (int j = 0; j < latitudeSegments; ++j) {
                    float phi = glm::pi<float>() * j / latitudeSegments;
                    vertices.emplace_back(center + radius * glm::vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta)));
                    phi = glm::pi<float>() * (j + 1) / latitudeSegments;
                    vertices.emplace_back(center + radius * glm::vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta)));
                }
            }

            // 生成纬线
            for (int j = 0; j < latitudeSegments; ++j) {
                float phi = glm::pi<float>() * j / latitudeSegments;
                const float sinPhi = sin(phi);
                const float cosPhi = cos(phi);
                for (int i = 0; i <= longitudeSegments; ++i) {
                    float theta = glm::two_pi<float>() * i / longitudeSegments;
                    vertices.emplace_back(center + radius * glm::vec3(sinPhi * cos(theta), cosPhi, sinPhi * sin(theta)));
                    theta = glm::two_pi<float>() * (i + 1) / longitudeSegments;
                    vertices.emplace_back(center + radius * glm::vec3(sinPhi * cos(theta), cosPhi, sinPhi * sin(theta)));
                }
            }
            return vertices;
        }

        std::vector<glm::vec3> operator()(const BoundingAABB& aabb) {
            std::vector<glm::vec3> vertices;
            const glm::vec3 min = aabb.center - aabb.extents;
            const glm::vec3 max = aabb.center + aabb.extents;

            const glm::vec3 v[] = {
                {min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z},
                {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z}
            };

            vertices.insert(vertices.end(), { v[0],v[1], v[1],v[2], v[2],v[3], v[3],v[0] });
            vertices.insert(vertices.end(), { v[4],v[5], v[5],v[6], v[6],v[7], v[7],v[4] });
            vertices.insert(vertices.end(), { v[0],v[4], v[1],v[5], v[2],v[6], v[3],v[7] });
            return vertices;
        }
    };

    static inline std::vector<glm::vec3> GenerateBoundingWireframe(
        const BoundingVolume& volume,
        int longitudeSegments = 16,
        int latitudeSegments = 16
    )
    {
        WireframeGenerator generator{ longitudeSegments, latitudeSegments };
        return std::visit(generator, volume);
    }

    Plane NormalizePlane(const glm::vec4& p);
    void ExtractPlanes(Plane planes[6], const glm::mat4& m); // 从view和projection矩阵中提取的六个平面

    struct PerformanceTime {
        float frameTime;
        float renderTime;
        float physicsTime;
    };
#define NOW() std::chrono::high_resolution_clock::now()
#define MEASURE_START() auto measure_start_##__LINE__ = NOW()
#define MEASURE_DURATION() \
    std::chrono::duration_cast<std::chrono::microseconds>(NOW() - measure_start_##__LINE__).count() / 1000.0f

}
#endif // TOOLS_H