/*****************************************************************//**
 * \file   assimpInstanceLoader.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef ASSIMPINSTANCELOADER_H
#define ASSIMPINSTANCELOADER_H
#include "../../render/core.h"
#include "../../render/object.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "../../render/mesh/InstanceMesh.h"
#include "../../resource/texture/texture.h"

class AssimpInstanceLoader {
public:
    // 使用智能指针返回加载的 Object 对象
    static std::shared_ptr<Object> load(const std::string& path, const unsigned int instanceCount);

private:
    // 递归处理每个节点，并将处理后的对象添加到 parent 中
    static void processNode(aiNode* ainode,
        std::shared_ptr<Object> parent,
        const aiScene* scene,
        const std::string& rootPath,
        const unsigned int instanceCount);

    // 处理 InstanceMesh，并返回相应的 InstanceMesh 对象
    static std::shared_ptr<InstanceMesh> processMesh(aiMesh* aimesh,
        const aiScene* scene,
        const std::string& rootPath,
        const unsigned int instanceCount);

    // 处理纹理，返回相应的 Texture 对象
    static std::shared_ptr<Texture> processTexture(const aiMaterial* aimat,
        const aiTextureType& type,
        const aiScene* scene,
        const std::string& rootPath,
        const unsigned int instanceCount);

    // 将 Assimp 的 aiMatrix4x4 转换为 glm::mat4
    static glm::mat4 getMat4f(aiMatrix4x4 value);
};
#endif // ASSIMPINSTANCELOADER_H