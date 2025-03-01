/*****************************************************************//**
 * \file   main.cpp
 * \brief  程序入口文件
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/


#include "util/common/preprocessor.h"
#ifdef VLD_CHECK
#include <vld.h>
#endif // VLD_CHECK
#include "render/core.h"
#include "render/shader.h"
#include "util/common/opengl.h"
#include "application/Application.h"
#include "resource/texture/texture.h"

#include "application/camera/perspectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/camera/trackBallCameraControl.h"
#include "application/camera/GameCameraControl.h"
#include "application/camera/vehicleCameraControl.h"

#include "render/geometry.h"
#include "render/material/phongMaterial.h"
#include "render/material/whiteMaterial.h"

#include "render/mesh/mesh.h"
#include "render/renderer/renderer.h"
#include "render/light/pointLight.h"
#include "render/light/spotLight.h"

#include "render/scene.h"
#include "resource/model/assimpLoader.h"
#include "render/material/depth.h"
#include "render/material/opacityMaskMaterial.h"
#include "render/material/screenMaterial.h"
#include "render/framebuffer/FrameBuffer.h"
#include "render/material/cubeMaterial.h"
#include "render/material/phongEnvMaterial.h"
#include "render/material/phongInstanceMaterial.h"
#include "render/material/GrassInstanceMaterial.h"
#include "render/mesh/InstanceMesh.h"
#include "./physics/physx_tool.h"
#include "util/profiler/benchmark.h"
#include "util/logger/log.h"
#include "core/EventBus.h"
#include "util/profiler/UnitTest.h"

std::unique_ptr<Renderer> renderer = nullptr;
std::shared_ptr<Scene> sceneInscreen = nullptr;

std::shared_ptr<DirectionalLight> dirLight = nullptr;

std::shared_ptr<AmbientLight> ambLight = nullptr;

std::shared_ptr<Camera> camera = nullptr;
std::shared_ptr<CameraControlType> cameraControl = nullptr;
std::shared_ptr<GrassInstanceMaterial> grassMaterial = nullptr;

glm::vec3 clearColor{ 0.6f, 0.7f, 0.6f };

// 记录上一帧的时间，用于计算帧率
std::shared_ptr<FrameBuffer> framebuffer = nullptr;
std::shared_ptr<FrameBuffer> fboResolve = nullptr;

using namespace Benchmark;

void OnResize(int width, int height) {
	glApp.setWindowSize(width, height);
	// 更新FBO
	framebuffer.reset(new FrameBuffer(width, height));
	std::dynamic_pointer_cast<ScreenMaterial>(std::dynamic_pointer_cast<Mesh>(sceneInscreen->getChildren()[0])->getMaterial())->setScreenTexture(framebuffer->getColorAttachment());
	GL_CALL(glViewport(0, 0, width, height));
}

void OnKey(int key, int /*scancode*/, int action, int mods) {
	if (action == SDL_KEYDOWN || action == SDL_KEYUP) {
		// 处理油门、后退、转向
		switch (key) {
		case SDLK_KP_8:
			handleThrottle(action); // forward
			break;
		case SDLK_KP_2:
#ifdef AUTOGEAR
			handleBackward(action); // backward
#endif
			break;
		case SDLK_KP_4:
			handleSteering(action, true); // true left
			break;
		case SDLK_KP_6:
			handleSteering(action, false); // false right
			break;
		case SDLK_SPACE:
			createDynamic(PxTransform(PxVec3(0, 20, 2), PxQuat(1.0f, 0.0f, 0.0f, 0.0f)), PxSphereGeometry(0.5f), PxVec3(0., 1., 0.) * 10.0f);
			break;
		case SDLK_q:
#ifdef MANUALGEAR
			if (action == SDL_KEYDOWN && gVehicle.mTransmissionCommandState.clutch == 0.0f) {
				gVehicle.mTransmissionCommandState.clutch = 1.0f;  // clutch
			}
			if (action == SDL_KEYUP && gVehicle.mTransmissionCommandState.clutch == 1.0f) {
				gVehicle.mTransmissionCommandState.clutch = 0.0f;  // release clutch
			}
#endif
			break;
		case SDLK_w: // 手动挡 升档
#ifdef MANUALGEAR
			if (action == SDL_KEYDOWN && gVehicle.mTransmissionCommandState.clutch == 1.0f && gVehicle.mEngineDriveState.gearboxState.currentGear != 6) {
				gVehicle.mTransmissionCommandState.targetGear = ++gVehicle.mEngineDriveState.gearboxState.currentGear;
			}
#endif
			break;
		case SDLK_e:
#ifdef MANUALGEAR
			if (action == SDL_KEYDOWN && gVehicle.mTransmissionCommandState.clutch == 1.0f && gVehicle.mEngineDriveState.gearboxState.currentGear != 0) {
				gVehicle.mTransmissionCommandState.targetGear = --gVehicle.mEngineDriveState.gearboxState.currentGear;
			}
#endif
			break;
		default:
			break;
		}
		cameraControl->onKey(key, action, mods);
	}
}

void OnMouse(int button, int action, double x, double y) {
	cameraControl->onMouse(button, action, x, y);
}

void OnCursor(double xpos, double ypos) {
	cameraControl->onCursor(xpos, ypos);
}

void OnScroll(double offset) {
	cameraControl->onScroll(static_cast<float>(offset));
}

// 设置模型颜色混合
void setModelBlend(std::shared_ptr<Object> object, bool blend, float opacity)
{
	if (object->getType() == ObjectType::Mesh)
	{
		auto mesh = std::dynamic_pointer_cast<Mesh>(object);
		mesh->getMaterial()->setBlend(blend);
		mesh->getMaterial()->setOpacity(opacity);
		mesh->getMaterial()->setDepthWrite(false);
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setModelBlend(children[i], blend, opacity);
	}
}
// 设置某个实例物体的位置
void setInstanceMatrix(std::shared_ptr<Object> object, int index, glm::mat4 matrix)
{
	if (object->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(object);
		(*mesh)[index] = matrix;
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setInstanceMatrix(children[i], index, matrix);
	}
}

void updateInstanceMatrix(std::shared_ptr<Object> object)
{
	if (object->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(object);
		mesh->updateMatrices();
	}
	auto children = object->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		updateInstanceMatrix(children[i]);
	}
}
// 为指定模型添加材质
void setInstanceMaterial(std::shared_ptr<Object> obj, std::shared_ptr<Material> material)
{
	if (obj->getType() == ObjectType::InstanceMesh)
	{
		auto mesh = std::dynamic_pointer_cast<InstanceMesh>(obj);
		mesh->setMaterial(material);
	}
	auto children = obj->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		setInstanceMaterial(children[i], material);
	}
}

void prepare() {
	renderer = std::make_unique<Renderer>();
	sceneInscreen = std::make_shared<Scene>();
	scene = std::make_shared<Scene>();

	framebuffer = FrameBuffer::createMultiSampleFbo(glApp.getWidth(), glApp.getHeight(), 8);
	fboResolve = std::make_shared<FrameBuffer>(glApp.getWidth(), glApp.getHeight());

	auto geometry2 = Geometry::createBox(1.0f);
	// 离屏渲染的box
	auto materialA = std::make_shared<CubeMaterial>(); // 天空盒材质
	materialA->setDiffuse(std::make_shared<Texture>("assets/textures/bk.jpg", 0, GL_SRGB_ALPHA, false));
	auto meshA = std::make_shared<Mesh>(geometry2, materialA);
	scene->addChild(meshA);

	//贴到屏幕上的矩形
	auto geo = Geometry::createScreenPlane();
	auto mat = std::make_shared<ScreenMaterial>();
	mat->setScreenTexture(fboResolve->getColorAttachment()); // !!!重要!!!!
	auto mesh = std::make_shared<Mesh>(geo, mat);
	sceneInscreen->addChild(mesh);

	//auto sphereGeo = Geometry::createSphere(1.0f);
	//auto sphereMat = std::make_shared<PhongInstanceMaterial>();
	//sphereMat->setDiffuse(std::make_shared<Texture>("assets/textures/earth.png", 0, GL_SRGB_ALPHA));

	dirLight = std::make_shared<DirectionalLight>();
	dirLight->mDirection = glm::vec3(-1.0f);
	dirLight->mSpecularIntensity = 0.5f;

	ambLight = std::make_shared<AmbientLight>();
	ambLight->mColor = glm::vec3(0.3f);
}

void prepareCamera() {
	camera = std::make_shared<PerspectiveCamera>(60.0f, (float)glApp.getWidth() / (float)glApp.getHeight(), 1.0f, 1000.0f);
	cameraControl = std::make_shared<CameraControlType>();
	cameraControl->setCamera(camera);
	cameraControl->setSensitivity(0.07f);
	// cameraControl->setSpeed(0.05f);
	camera->mPosition = glm::vec3(0.0f, 2.5f, 12.0f);
}

void initIMGUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext(); // 创建imgui上下文
	ImGui::StyleColorsLight(); // 选择一种主题

	// 初始化ImGui与GLFW和OpenGL的接口
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui_ImplSDL2_InitForOpenGL(glApp.getWindow(), glApp.getContext());
}

void renderIMGUI()
{
	// 创建一个新的IMGUI帧
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// 创建UI并放置一些控件
	ImGui::Begin("OpenGL"); // 创建一个新的窗口
	ImGui::Text("FPS: %.2f", io.Framerate); // 显示帧率
	// 速度
	{
		// 获取小车的线性速度
		const PxVec3 linVel1 = gVehicle.mPhysXState.physxActor.rigidBody->getLinearVelocity();
		// 计算速度的大小（即速度的模长）
		float speedMagnitude = linVel1.magnitude();
		float speedKmh = speedMagnitude * 3.6f; // 转成km/h
		ImGui::Text("Speed: %.2f km/h", speedKmh); // 显示速度的大小
	}
	// 挡位
	{
		ImGui::Text("Current Gear: %d", gVehicle.mEngineDriveState.gearboxState.currentGear); // 显示当前挡位
	}

	ImGui::End();
	// 执行UI渲染
	ImGui::Render();
	// 获取当前窗口的宽高
	int display_w = 0, display_h = 0;
	SDL_GL_GetDrawableSize(glApp.getWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	// 渲染ImGui的绘制数据
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(int argc, char* argv[]) {
#ifdef DEBUG
	LOG_INIT(LOG_LEVEL_DEBUG, AppenderType::APD_CONSOLE | AppenderType::APD_FILE, "log");
#else
	LOG_INIT(LOG_LEVEL_INFO, AppenderType::APD_FILE, "log");
#endif // DEBUG

#ifdef UNIT_T
	/* Unit Test Enabled */
	LOG_INFO("Enable Unit Test");
	UNIT_TEST;
#endif // UNIT_T

	glApp.init(1280, 768, "ImGui+SDL2+OpenGL+PhysX");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 设置回调函数
	glApp.setResizeCallBack(OnResize);
	glApp.setKeyCallBack(OnKey);
	glApp.setMouseCallBack(OnMouse);
	glApp.setCursorCallBack(OnCursor);
	glApp.setScrollCallBack(OnScroll);

	// 初始化OpenGL接口和清空颜色
	GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	prepareCamera(); // 准备相机
	// 准备帧缓冲对象
	prepare(); // 准备其他资源
	initIMGUI(); // 初始化IMGUI
	if (!initPhysics()) // 初始化物理引擎
		LOG_ERROR("Failed to initialize PhysX");
	preparePhysics(); // 准备渲染图形

	while (glApp.update()) {
		stepPhysics();
		const int MAX_NUM_ACTOR_SHAPES = 128;

		PxU32 nbActors = gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		PxArray<PxRigidActor*> actors(nbActors);
		gScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

		PxShape* shapes[MAX_NUM_ACTOR_SHAPES] = { 0 };

		// 定义跳过的物体类型，例如地面可以作为静态物体跳过
		const PxActor* skipActor = actors[0];  // 假设第一个物体是地面，需跳过
		PxU32 sceneChildIndex = 2;  // 从场景的第三个对象开始设置模型矩阵，假设前两个是地面和背景等

		for (PxU32 i = 0; i < static_cast<PxU32>(actors.size()); i++)
		{
			// 跳过不需要的物体
			if (actors[i] == skipActor)
			{
				continue;  // 跳过地面或其他不需要的物体
			}
			const PxU32 nbShapes = actors[i]->getNbShapes();
			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
			actors[i]->getShapes(shapes, nbShapes);
			//printf("Actor %d has %d shapes\n", i, nbShapes);
			for (PxU32 j = 0; j < nbShapes; j++)  // 遍历每个形状
			{
				// 获取形状的变换矩阵
				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
				// 获取形状的几何信息
				const PxGeometry& shapeGeometry = shapes[j]->getGeometry();

				// 判断物体类型，根据不同形状设置模型矩阵
				switch (shapeGeometry.getType())
				{
				case PxGeometryType::eBOX:
				{
					// 设置模型矩阵
					if (scene->getChildren().size() > sceneChildIndex)
					{
						scene->getChildren()[sceneChildIndex]->setModelMatrix(glm::make_mat4(&shapePose.column0.x));
						sceneChildIndex++;  // 增加下一个模型矩阵的索引
					}
					break;
				}
				case PxGeometryType::eCONVEXMESH:
				{
					// 设置模型矩阵
					if (scene->getChildren().size() > sceneChildIndex)
					{
						scene->getChildren()[sceneChildIndex]->setModelMatrix(glm::make_mat4(&shapePose.column0.x));
						sceneChildIndex++;  // 增加下一个模型矩阵的索引
					}
					break;
				}
				case PxGeometryType::eSPHERE:
				{
					if (scene->getChildren().size() > sceneChildIndex)
					{
						scene->getChildren()[sceneChildIndex]->setModelMatrix(glm::make_mat4(&shapePose.column0.x));
						sceneChildIndex++;  // 增加下一个模型矩阵的索引
					}
					break;
				}
				default:
					break;
				}
			}
		}

		if (auto temp = std::dynamic_pointer_cast<VehicleCameraControl>(cameraControl))
		{
			// 开启相机跟随
			glm::mat4 Model = scene->getChildren()[2]->getModelMatrix();
			glm::vec3 targetPosition = glm::vec3(Model[3]); // 获取模型矩阵的平移部分，即物体的位置
			// 提取小车的前向方向（模型矩阵的Z轴为前进方向）
			glm::vec3 forward = glm::normalize(glm::vec3(Model[2]));

			// 计算相机参数
			float distanceBehind = 7.0f; // 距离小车后方距离
			float height = 2.8f;         // 相机高度偏移
			const glm::vec3 cameraPos = targetPosition - forward * distanceBehind + glm::vec3(0, height, 0);
			const glm::vec3 cameraTarget = targetPosition + forward; // 看向小车前方

			temp->setCameraPosTarget(cameraPos, cameraTarget); // 设置相机位置和目标点
		}
		cameraControl->update(); // 更新相机控制

		renderer->setClearColor(clearColor); // 设置渲染器的清除颜色
		renderer->render(scene, camera, dirLight, ambLight, framebuffer->getFBO()); // 渲染到离屏缓冲区
		renderer->msaaResolve(framebuffer, fboResolve);
		renderer->render(sceneInscreen, camera, dirLight, ambLight); // 渲染到屏幕
		renderIMGUI(); // 渲染IMGUI界面

	}
	// PhysX
	releasePhysics();
	// SDL2
	glApp.destroy();

	return 0;
}