/*****************************************************************//**
 * \file   renderer.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include "../core.h"
#include "../mesh/mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"

class FrameBuffer;
class Renderer {
public:
    Renderer();
    ~Renderer();

    void render(
        std::shared_ptr<Scene> scene,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<DirectionalLight> dirLight,
        std::shared_ptr<AmbientLight> ambLight,
        unsigned int fbo = 0
        , glm::vec3 center = glm::vec3(0.0f), float radius = 0.0f
    );

    void renderObject(
        std::shared_ptr<Object> object,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<DirectionalLight> dirLight,
        std::shared_ptr<AmbientLight> ambLight
    );

    void setClearColor(glm::vec3 color);
    void msaaResolve(std::shared_ptr<FrameBuffer> src, std::shared_ptr<FrameBuffer> dst);

    void InitAABBRenderer();
    void DrawAABB(const std::array<glm::vec3, 8>& worldCorners, const glm::mat4& viewProj);
public:
    std::shared_ptr<Material> mGlobalMaterial;

private:
    void projectObject(std::shared_ptr<Object> obj);

    std::shared_ptr<Shader> pickShader(MaterialType type);

    void setDepthState(std::shared_ptr<Material> material);
    void setStencilState(std::shared_ptr<Material> material);
    void setBlendState(std::shared_ptr<Material> material);
    void setFaceCullState(std::shared_ptr<Material> material);
    void setPolygonOffsetState(std::shared_ptr<Material> material);
private:
    std::shared_ptr<Shader> mPhongShader;
    std::shared_ptr<Shader> mWhiteShader;
    std::shared_ptr<Shader> mDepthShader;
    std::shared_ptr<Shader> mOpacityMaskShader;
    std::shared_ptr<Shader> mScreenShader;
    std::shared_ptr<Shader> mCubeShader;
    std::shared_ptr<Shader> mPhongEnvShader;
    std::shared_ptr<Shader> mPhongInstShader;
    std::shared_ptr<Shader> mGrassInstShader;
    std::shared_ptr<Shader> mLineShader;

private:
    std::vector<std::shared_ptr<Mesh>> mOpacityObjects; // 半透明不透明物体
    std::vector<std::shared_ptr<Mesh>> mTransparentObjects; // 透明物体
};
#endif // RENDERER_H