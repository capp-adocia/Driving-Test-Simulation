/*****************************************************************//**
 * \file   renderer.cpp
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"
#include "../material/whiteMaterial.h"
#include "../material/depth.h"
#include "../material/opacityMaskMaterial.h"
#include "../material/screenMaterial.h"
#include "../material/cubeMaterial.h"
#include "../material/phongEnvMaterial.h"
#include "../material/phongInstanceMaterial.h"
#include "../material/GrassInstanceMaterial.h"
#include "../material/phongNormalMaterial.h"
#include "../mesh/InstanceMesh.h"
#include "../framebuffer/FrameBuffer.h"
#include <string>
#include <algorithm>
#include "../../util/logger/log.h"

Renderer::Renderer() 
	: mGlobalMaterial(nullptr)
	, mPhongShader(std::make_shared<Shader>("assets/shaders/phong.vert", "assets/shaders/phong.frag"))
	, mWhiteShader(std::make_shared<Shader>("assets/shaders/white.vert", "assets/shaders/white.frag"))
	, mDepthShader(std::make_shared<Shader>("assets/shaders/depth.vert", "assets/shaders/depth.frag"))
	, mOpacityMaskShader(std::make_shared<Shader>("assets/shaders/phongMask.vert", "assets/shaders/phongMask.frag"))
	, mScreenShader(std::make_shared<Shader>("assets/shaders/screen.vert", "assets/shaders/screen.frag"))
	, mCubeShader(std::make_shared<Shader>("assets/shaders/cube.vert", "assets/shaders/cube.frag"))
	, mPhongEnvShader(std::make_shared<Shader>("assets/shaders/phongEnv.vert", "assets/shaders/phongEnv.frag"))
	, mPhongInstShader(std::make_shared<Shader>("assets/shaders/phongInstance.vert", "assets/shaders/phongInstance.frag"))
	, mGrassInstShader(std::make_shared<Shader>("assets/shaders/grassInstance.vert", "assets/shaders/grassInstance.frag"))
	, mLineShader(std::make_shared<Shader>("assets/shaders/line.vert", "assets/shaders/line.frag"))
	, mPhongNormalShader(std::make_shared<Shader>("assets/shaders/phongNormal.vert", "assets/shaders/phongNormal.frag"))
	, mOpacityObjects({})
	, mTransparentObjects({})
{
	InitAABBRenderer();
}

Renderer::~Renderer() {

}

void Renderer::setClearColor(glm::vec3 color) {
	glClearColor(color.r, color.g, color.b, 1.0);
}

// MASS
void Renderer::msaaResolve(std::shared_ptr<FrameBuffer> src, std::shared_ptr<FrameBuffer> dst)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getFBO());
	glBlitFramebuffer(0, 0, src->getWidth(), src->getHeight(), 0, 0, dst->getWidth(), dst->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

static GLuint vaoSphere, vboSphere;
void Renderer::InitAABBRenderer() {
	glGenVertexArrays(1, &vaoSphere);
	glGenBuffers(1, &vboSphere);
	glBindVertexArray(vaoSphere);
	glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::render(
	std::shared_ptr<Scene> scene,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight,
	unsigned int fbo
)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// 设置当前帧绘制的时候，opengl的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glDisable(GL_POLYGON_OFFSET_FILL); // 关闭填充多边形的偏移
	glDisable(GL_POLYGON_OFFSET_LINE); // 关闭绘制线框多边形的偏移
	// 开启测试、设置基本写入状态、打开模板测试写入
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF); // 开启模板测试写入，保证可以被清理掉模板缓冲

	// 关闭混合
	glDisable(GL_BLEND);

	// 清理画布 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	mOpacityObjects.clear();
	mTransparentObjects.clear();
	
	// 将不透明和透明物体分开，装到不同容器中
	projectObject(scene);

	std::sort(mTransparentObjects.begin(), mTransparentObjects.end()
		, [camera](const std::shared_ptr<Mesh> a, const std::shared_ptr<Mesh> b) {
			auto viewMatrix = camera->getViewMatrix();
			// A
			auto modelMatrixA = a->getModelMatrix();
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;
			// B
			auto modelMatrixB = b->getModelMatrix();
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z; // z值越小，越在前面
		});


	// 渲染两个队列
	for (auto obj : mOpacityObjects)
	{
		renderObject(obj, camera, dirLight, ambLight);
	}

	for (auto obj : mTransparentObjects)
	{
		renderObject(obj, camera, dirLight, ambLight);
	}
}

void Renderer::projectObject(std::shared_ptr<Object> obj)
{
	// 绘制策略：先绘制不透明物体，再绘制透明物体
	if (obj->getType() == ObjectType::Mesh || obj->getType() == ObjectType::InstanceMesh)
	{
		std::shared_ptr<Mesh> mesh = std::static_pointer_cast<Mesh>(obj);
		std::shared_ptr<Material> material = mesh->getMaterial();
		if (material->getBlend())
		{
			mTransparentObjects.push_back(mesh);
		}
		else
		{
			mOpacityObjects.push_back(mesh);
		}
	}
	auto children = obj->getChildren();
	for (auto child : children)
	{
		projectObject(child);
	}
}

std::shared_ptr<Shader> Renderer::pickShader(MaterialType type)
{
	std::shared_ptr<Shader> result = nullptr;
	switch (type) {
	case MaterialType::PhongMaterial:
		result = mPhongShader;
		break;
	case MaterialType::WhiteMaterial:
		result = mWhiteShader;
		break;
	case MaterialType::DepthMaterial:
		result = mDepthShader;
		break;
	case MaterialType::OpacityMaskMaterial:
		result = mOpacityMaskShader;
		break;
	case MaterialType::ScreenMaterial:
		result = mScreenShader;
		break;
	case MaterialType::CubeMaterial:
		result = mCubeShader;
		break;
	case MaterialType::PhongEnvMaterial:
		result = mPhongEnvShader;
		break;
	case MaterialType::PhongInstanceMaterial:
		result = mPhongInstShader;
		break;
	case MaterialType::GrassInstanceMaterial:
		result = mGrassInstShader;
		break;
	case MaterialType::PhongNormalMaterial:
	{
		result = mPhongNormalShader;
		break;
	}
	default:
		LOG_WARN("Unknown material type to pick shader");
		break;
	}
	return result;
}

// 设置深度检测的状态
void Renderer::setDepthState(std::shared_ptr<Material> material)
{
	if (material->getDepthTest())
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(material->getDepthFunc());
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	material->getDepthWrite() ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
}

void Renderer::setStencilState(std::shared_ptr<Material> material)
{
	if (material->getStencilTest())
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(material->getSFail(), material->getZFail(), material->getZPass());
		glStencilMask(material->getStencilMask());
		glStencilFunc(material->getStencilFunc(), material->getStencilRef(), material->getStencilFuncMask());
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::setBlendState(std::shared_ptr<Material> material)
{
	if (material->getBlend())
	{
		glEnable(GL_BLEND);
		glBlendFunc(material->getSFactor(), material->getDFactor());
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

void Renderer::setFaceCullState(std::shared_ptr<Material> material)
{
	if (material->getFaceCulling())
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(material->getFrontFace());
		glCullFace(material->getCullFace());
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
void Renderer::setPolygonOffsetState(std::shared_ptr<Material> material)
{
	if (material->getPolygonOffset()) {
		glEnable(material->getPolygonOffsetType());
		glPolygonOffset(material->getFactor(), material->getUnit());
	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);
	}
}
static int b = 0;
static int target = 0;
//针对单个object进行渲染
void Renderer::renderObject(
	std::shared_ptr<Object> object,
	std::shared_ptr<Camera> camera,
	std::shared_ptr<DirectionalLight> dirLight,
	std::shared_ptr<AmbientLight> ambLight
)
{
	glm::vec3 worldCenter = glm::vec3(0.0f);
	float worldRadius = 0.0f;
	//1 判断是Mesh还是Object，如果是Mesh需要渲染，是InstanceMesh也要判断
	if (object->getType() == ObjectType::Mesh || object->getType() == ObjectType::InstanceMesh) {
		auto mesh = std::dynamic_pointer_cast<Mesh>(object);
		auto geometry = mesh->getGeometry();

		std::shared_ptr<Material> material = nullptr;
		if (mGlobalMaterial) material = mGlobalMaterial;
		else material = mesh->getMaterial();
		// 设置多边形偏移状态
		setPolygonOffsetState(material);
		// 设置深度检测的状态
		setDepthState(material);
		// 设置模板检测的状态
		setStencilState(material);
		// 默认颜色混合
		setBlendState(material);
		// 面剔除
		setFaceCullState(material);

		/* !视锥体剔除 */
		if(material->getType() != MaterialType::CubeMaterial && material->getType() != MaterialType::ScreenMaterial)
		{
			glm::mat4 modelMatrix = mesh->getModelMatrix();
			worldCenter = glm::vec3(modelMatrix * glm::vec4(geometry->boundingSphereCenter, 1.0f));
			worldRadius = geometry->boundingSphereRadius * glm::max(glm::length(modelMatrix[0]), glm::max(glm::length(modelMatrix[1]), glm::length(modelMatrix[2])));
			glm::mat4 viewProj = camera->getProjectionMatrix() * camera->getViewMatrix();
			Util::Plane planes[6];
			Util::ExtractPlanes(planes, viewProj);
			if (!Util::SphereInFrustum(worldCenter, worldRadius, planes))
				return;
		}
		// TODO: 根据物体是否需要渲染AABB来决定是否渲染
		if (mesh->ShowAABB())
		{
			// 渲染 AABB 线框
			glm::mat4 viewProj = camera->getProjectionMatrix() * camera->getViewMatrix();
			mLineShader->begin();
			{
				// 生成球体线框顶点
				std::vector<glm::vec3> vertices = Util::GenerateSphereWireframe(worldCenter, worldRadius, 8, 16);
				mLineShader->setMatrix4x4("viewProj", viewProj);
				mLineShader->setVector3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));
				glBindVertexArray(vaoSphere);
				glBindBuffer(GL_ARRAY_BUFFER, vboSphere);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
				glDrawArrays(GL_LINES, 0, (int)vertices.size());
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
			mLineShader->end();
		}
		// 决定使用哪个Shader 
		std::shared_ptr<Shader> shader = pickShader(material->getType());
		// 更新shader的uniform
		shader->begin();
		switch (material->getType()) {
		case MaterialType::PhongMaterial: {
			std::shared_ptr<PhongMaterial> phongMat = std::dynamic_pointer_cast<PhongMaterial>(material);
			//将纹理采样器与纹理单元进行挂钩
			shader->setInt("sampler", 0);
			//将纹理与纹理单元进行挂钩
			phongMat->getDiffuse()->bind();

			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

			shader->setFloat("shiness", phongMat->mShiness);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("opacity", phongMat->getOpacity());

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			break;
		}
		case MaterialType::WhiteMaterial: {
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
			break;
		}
		case MaterialType::DepthMaterial: {
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			shader->setFloat("near", camera->mNear);
			shader->setFloat("far", camera->mFar);
			break;
		}
		case MaterialType::OpacityMaskMaterial: {
			std::shared_ptr<OpacityMaskMaterial> opacityMat = std::dynamic_pointer_cast<OpacityMaskMaterial>(material);

			shader->setInt("sampler", 0);
			opacityMat->getDiffuse()->bind();

			shader->setInt("opacityMaskSampler", 1);
			opacityMat->getSpecularMask()->bind(); // 透明度蒙版

			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			shader->setFloat("shiness", opacityMat->mShiness);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("opacity", opacityMat->getOpacity());

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
			break;
		}
		case MaterialType::ScreenMaterial: {
			std::shared_ptr<ScreenMaterial> screenMat = std::dynamic_pointer_cast<ScreenMaterial>(material);
			shader->setInt("screenTexture", 0);
			screenMat->getScreenTexture()->bind();
			break;
		}
		case MaterialType::CubeMaterial: {
			std::shared_ptr<CubeMaterial> cubeMat = std::dynamic_pointer_cast<CubeMaterial>(material);
			mesh->setPosition(camera->mPosition);
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
			shader->setInt("sphericalSampler", 0);
			cubeMat->getDiffuse()->bind();
			break;
		}
		case MaterialType::PhongEnvMaterial:
		{
			std::shared_ptr<PhongEnvMaterial> phongEnvMat = std::dynamic_pointer_cast<PhongEnvMaterial>(material);
			shader->setInt("sampler", 0);
			phongEnvMat->getDiffuse()->bind();
			shader->setInt("envSampler", 1);
			phongEnvMat->getEnv()->bind();
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			shader->setFloat("shiness", phongEnvMat->mShiness);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("opacity", phongEnvMat->getOpacity());
			shader->setVector3("cameraPosition", camera->mPosition);
			break;
		}
		case MaterialType::PhongInstanceMaterial:
		{
			std::shared_ptr<PhongInstanceMaterial> phongMat = std::dynamic_pointer_cast<PhongInstanceMaterial>(material);
			std::shared_ptr<InstanceMesh> instanceMesh = std::dynamic_pointer_cast<InstanceMesh>(mesh);
			shader->setInt("sampler", 0);
			phongMat->getDiffuse()->bind();

			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

			shader->setFloat("shiness", phongMat->getShiness());
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("opacity", phongMat->getOpacity());

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);
			break;
		}
		case MaterialType::GrassInstanceMaterial:
		{
			std::shared_ptr<GrassInstanceMaterial> grassMat = std::dynamic_pointer_cast<GrassInstanceMaterial>(material);
			std::shared_ptr<InstanceMesh> instanceMesh = std::dynamic_pointer_cast<InstanceMesh>(mesh);
			shader->setInt("sampler", 0);
			grassMat->getDiffuse()->bind();

			shader->setInt("opacityMask", 1);
			grassMat->getOpacityMask()->bind();

			instanceMesh->sortMatrices(camera->getViewMatrix());
			instanceMesh->updateMatrices(); // 排完序后必须马上更新

			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix()); // 对原坐标 + 变换
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
			// 实例绘制不能再这里更新normalMatrix了
			/*auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);*/

			//光源参数的uniform更新
			//directionalLight 的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

			shader->setFloat("shiness", grassMat->mShiness);
			shader->setVector3("ambientColor", ambLight->mColor);
			shader->setFloat("opacity", grassMat->getOpacity());

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			shader->setFloat("uvScale", grassMat->getUVScale());
			shader->setFloat("lightIntensity", grassMat->getLightIntensity());

			Uint64 counter = SDL_GetPerformanceCounter();
			Uint64 frequency = SDL_GetPerformanceFrequency();
			float time = (float)counter / (float)frequency; // 计算时间（秒）
			shader->setFloat("time", time);

			shader->setFloat("windScale", grassMat->getWindScale());
			shader->setFloat("phaseScale", grassMat->getPhaseScale());
			shader->setVector3("windDirection", grassMat->getWindDirection());

			// 云层
			shader->setInt("cloudMask", 2);
			grassMat->getCloudMask()->bind();
			shader->setVector3("cloudWhiteColor", grassMat->getCloudWhite());
			shader->setVector3("cloudBlackColor", grassMat->getCloudBlack());
			shader->setFloat("cloudUVScale", grassMat->getCloudUVScale());
			shader->setFloat("cloudSpeed", grassMat->getCloudSpeed());
			shader->setFloat("cloudLerp", grassMat->getCloudLerp());
			break;
		}
		case MaterialType::PhongNormalMaterial: {
			std::shared_ptr<PhongNormalMaterial> phongMat = std::dynamic_pointer_cast<PhongNormalMaterial>(material);

			//diffuse贴图帧更新
			shader->setInt("sampler", 0);
			phongMat->mDiffuse->bind();
			//法线贴图
			shader->setInt("normalMapSampler", 1);
			phongMat->mNormalMap->bind();
			//mvp
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			//光源参数的uniform更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);
			shader->setFloat("directionalLight.intensity", dirLight->mIntensity);

			shader->setFloat("shiness", phongMat->mShiness);

			shader->setVector3("ambientColor", ambLight->mColor);

			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			//透明度
			shader->setFloat("opacity", material->getOpacity());
			break;
		}
		default:
			break;
		}
		//3 绑定vao
		glBindVertexArray(geometry->getVao());

		// 4 执行绘制命令
		if (object->getType() == ObjectType::InstanceMesh)
		{
			std::shared_ptr<InstanceMesh> instanceMesh = std::dynamic_pointer_cast<InstanceMesh>(mesh);
			glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, instanceMesh->getInstanceCount());
		}
		else
		{
			// 单个绘制
			glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);
		shader->end();

	}

}