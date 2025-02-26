/*****************************************************************//**
 * \file   physx_tool.h
 * \brief  
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef PHYSX_TOOL_H
#define PHYSX_TOOL_H

#include <physx/PxPhysicsAPI.h>
#include "EngineDrivetrain.h"
#include "../application/assimpLoader.h"

using namespace physx;
using namespace physx::vehicle2;

/* Scene */
std::shared_ptr<Scene> scene = nullptr;

/* Physx */
//A ground plane to drive on.
PxRigidStatic* gGroundPlane = NULL;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;

PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;

std::shared_ptr<Object> model = nullptr;

struct physx_actor_entity
{
	PxRigidActor* actorPtr;
	PxU32 actorId;
};

std::vector<physx_actor_entity> physx_actors;

static PxU32 stackCounter = 0;

void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxVec3 vec3_temp = PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0);
			PxTransform localTm(vec3_temp * halfExtent); // 设置物体的位置，旋转默认
			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 20.0f);
			gScene->addActor(*body);

			physx_actors.push_back({ body, stackCounter++ });
		}
	}
	shape->release();
	// box创建
	auto boxGeometry = Geometry::createBox(2 * halfExtent);
	auto boxMeterial = std::make_shared<PhongMaterial>();
	boxMeterial->mDiffuse = std::make_shared<Texture>("assets/textures/box.png", 0, GL_SRGB_ALPHA);
	for (size_t i = 0; i < (size * (size + 1)) / 2; i++)
	{
		auto Box2 = std::make_shared<Mesh>(boxGeometry, boxMeterial);
		scene->addChild(Box2);
	}
}

void createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	static PxU32 dynamicCounter = 0;

	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	physx_actors.push_back({ dynamic, dynamicCounter++ });

	auto Geometry = Geometry::createSphere(0.5f);
	auto Material = std::make_shared<PhongMaterial>();
	Material->mDiffuse = std::make_shared<Texture>("assets/textures/wall.jpg", 0, GL_SRGB_ALPHA);
	auto sphere = std::make_shared<Mesh>(Geometry, Material);
	scene->addChild(sphere);
}

void stepPhysics();
void initPhysX();
void initGroundPlane();
void initMaterialFrictionTable();
bool initVehicles();

PxVehiclePhysXMaterialFriction gPhysXMaterialFrictions[16];
PxU32 gNbPhysXMaterialFrictions = 0;
PxReal gPhysXDefaultMaterialFriction = 1.0f;
//Gravitational acceleration
const PxVec3 gGravity(0.0f, -9.81f, 0.0f);
PxVehiclePhysXSimulationContext gVehicleSimulationContext;
//The vehicle with engine drivetrain
EngineDriveVehicle gVehicle; // 第一辆车

// 设置前进
void throttle()
{
	// 设置油门
	gVehicle.mCommandState.throttle = 0.8f;  // 油门
	gVehicle.mCommandState.brakes[0] = 0.0f;  // 刹车保持关闭
	gVehicle.mCommandState.brakes[1] = 0.0f;  // 刹车保持关闭

	gVehicle.mTransmissionCommandState.targetGear = PxVehicleEngineDriveTransmissionCommandState::eAUTOMATIC_GEAR;
}

// 设置后退
void backward()
{
	// 设置油门为负值，使车辆后退
	gVehicle.mCommandState.throttle = 1.0f;  // 油门
	gVehicle.mCommandState.brakes[0] = 0.0f;   // 刹车关闭
	gVehicle.mCommandState.brakes[1] = 0.0f;   // 刹车关闭

	gVehicle.mTransmissionCommandState.targetGear = 0;
}
// 刹车
void brake()
{
	gVehicle.mCommandState.throttle = 0.0f;  // 油门完全关闭
}

// 记录当前方向状态，-1 = 右转，1 = 左转，0 = 直行
int currentSteerDirection = 0;
// 设置左转
void steerLeft()
{
	// 设置左转
	gVehicle.mCommandState.steer = 0.15f;     // 左转
	gVehicle.mCommandState.brakes[0] = 0.0f;
	gVehicle.mCommandState.brakes[1] = 0.0f;
	currentSteerDirection = 1; // 更新当前方向
}

// 设置右转
void steerRight()
{
	// 设置右转
	gVehicle.mCommandState.steer = -0.15f;      // 右转
	gVehicle.mCommandState.brakes[0] = 0.0f;
	gVehicle.mCommandState.brakes[1] = 0.0f;
	currentSteerDirection = -1; // 更新当前方向
}

// 方向摆正
void straighten()
{
	gVehicle.mCommandState.steer = 0.0f;      // 方向摆正
	gVehicle.mCommandState.brakes[0] = 0.0f;
	gVehicle.mCommandState.brakes[1] = 0.0f;
	currentSteerDirection = 0; // 直行时更新方向状态
}
// 方向键按键函数
void handleThrottle(int action) {
	if (action == SDL_KEYDOWN) {
		throttle();
	}
	else if (action == SDL_KEYUP) {
		brake();
	}
}

void handleBackward(int action) {
	if (action == SDL_KEYDOWN) {
		backward();
	}
	else if (action == SDL_KEYUP) {
		brake();
	}
}
void handleSteering(int action, bool isLeft) {
	switch (action)
	{
	case SDL_KEYUP:
	{
		// 只有当前按下的是释放的方向键时才回正方向
		if (isLeft && currentSteerDirection == 1) {
			straighten();
		}
		else if (!isLeft && currentSteerDirection == -1) {
			straighten();
		}
		break;
	}
	case SDL_KEYDOWN:
	{
		// 按下左键时转向左，按下右键时转向右
		isLeft ? steerLeft() : steerRight();
		break;
	}
	default:
		break;
	}
}

// 模拟中更新
void stepPhysics()
{
	const PxReal timestep = 1.0f / 60.0f;

	// 第一辆车的物理步进
	gVehicle.mCommandState.nbBrakes = 1; // 设置刹车数量为1
	const PxVec3 linVel1 = gVehicle.mPhysXState.physxActor.rigidBody->getLinearVelocity();
	const PxVec3 forwardDir1 = gVehicle.mPhysXState.physxActor.rigidBody->getGlobalPose().q.getBasisVector2();
	const PxReal forwardSpeed1 = linVel1.dot(forwardDir1);

	const PxU8 nbSubsteps1 = (forwardSpeed1 < 5.0f ? 3 : 1);
	gVehicle.mComponentSequence.setSubsteps(gVehicle.mComponentSequenceSubstepGroupHandle, nbSubsteps1);
	gVehicle.step(timestep, gVehicleSimulationContext);

	gScene->simulate(timestep);
	gScene->fetchResults(true);
}

bool initPhysics()
{
	initPhysX();
	initGroundPlane();
	initMaterialFrictionTable();
	if (!initVehicles()) return false;
	return true;
}

PxFilterFlags VehicleFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(filterData0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData1);
	PX_UNUSED(pairFlags);
	PX_UNUSED(constantBlock);
	PX_UNUSED(constantBlockSize);
	pairFlags = PxPairFlag::eDETECT_DISCRETE_CONTACT | PxPairFlag::eSOLVE_CONTACT;
	return PxFilterFlag::eDEFAULT;
}

// 初始化物理引擎
void initPhysX()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = gGravity;
	gDispatcher = PxDefaultCpuDispatcherCreate(1);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = VehicleFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxInitVehicleExtension(*gFoundation);
}

// 初始化地面
void initGroundPlane()
{
	gGroundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	for (PxU32 i = 0; i < gGroundPlane->getNbShapes(); i++)
	{
		PxShape* shape = NULL;
		gGroundPlane->getShapes(&shape, 1, i);
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true); // 是否开启场景查询
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true); // 是否开启物理模拟
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, false); // 是否开启触发器
	}
	gScene->addActor(*gGroundPlane);
}

void initMaterialFrictionTable()
{
	gPhysXMaterialFrictions[0].friction = 1.0f; // 设置第一个摩擦力材质的摩擦力为最大值
	gPhysXMaterialFrictions[0].material = gMaterial;
	gPhysXDefaultMaterialFriction = 1.0f; // 设置默认的摩擦力为最大值
	gNbPhysXMaterialFrictions = 1; // 设置当前摩擦力材质的数量为 1
}
void LoadBaseParams(BaseVehicleParams& baseParams);
void LoadEngineDrivertrainParams(EngineDrivetrainParams& engineDriveParams);

bool initVehicles()
{
	// 现有的第一辆车的初始化
	LoadBaseParams(gVehicle.mBaseParams);
	setPhysXIntegrationParams(gVehicle.mBaseParams.axleDescription, gPhysXMaterialFrictions, gNbPhysXMaterialFrictions, gPhysXDefaultMaterialFriction, gVehicle.mPhysXParams);

	LoadEngineDrivertrainParams(gVehicle.mEngineDriveParams);
	if (!gVehicle.initialize(*gPhysics, PxCookingParams(PxTolerancesScale()), *gMaterial, EngineDriveVehicle::eDIFFTYPE_FOURWHEELDRIVE))
	{
		return false;
	}
	// 设置车辆的初始位置和方向
	PxTransform pose1(PxVec3(0.0f, 0.2f, 0.0f), PxQuat(PxIdentity));  // 第一辆车
	gVehicle.setUpActor(*gScene, pose1, "EngineDrive1");

	gVehicle.mEngineDriveState.gearboxState.currentGear = 1;
	gVehicle.mTransmissionCommandState.targetGear = PxVehicleEngineDriveTransmissionCommandState::eAUTOMATIC_GEAR;

	// 初始化模拟上下文
	gVehicleSimulationContext.setToDefault();
	gVehicleSimulationContext.frame.lngAxis = PxVehicleAxes::ePosZ;
	gVehicleSimulationContext.frame.latAxis = PxVehicleAxes::ePosX;
	gVehicleSimulationContext.frame.vrtAxis = PxVehicleAxes::ePosY;
	gVehicleSimulationContext.scale.scale = 1.0f;
	gVehicleSimulationContext.gravity = gGravity;
	gVehicleSimulationContext.physxScene = gScene;
	gVehicleSimulationContext.physxActorUpdateMode = PxVehiclePhysXActorUpdateMode::eAPPLY_ACCELERATION;

	return true;
}

void preparePhysics()
{
	/* opengl对应图形 */
	auto planeGeometry = Geometry::createBox(800.f, 0.1f, 800.f);
	auto planeMaterial = std::make_shared<PhongMaterial>();
	planeMaterial->mDiffuse = std::make_shared<Texture>("assets/textures/wall.jpg", 0, GL_SRGB_ALPHA);
	auto plane = std::make_shared<Mesh>(planeGeometry, planeMaterial);
	scene->addChild(plane);

	// 一个车体
	auto boxGeometry = Geometry::createBox(1.5905f, 1.140f, 2.8632f * 2 - 0.3432f); // 宽高长
	auto boxMaterial = std::make_shared<PhongMaterial>();
	boxMaterial->mDiffuse = std::make_shared<Texture>("assets/textures/box.png", 0, GL_SRGB_ALPHA);
	auto box = std::make_shared<Mesh>(boxGeometry, boxMaterial);
	scene->addChild(box);

	//model = AssimpLoader::load("assets/fbx/1.fbx");
	//model->setScale(glm::vec3(0.01f));
	//scene->addChild(model);
	//model->setAngleY(90.f);

	// 四个车轮
	for (int i = 0; i < 4; i++)
	{
		auto wheelGeometry = Geometry::createCylinder(0.3432f, 0.225f * 2.0f);
		auto wheelMaterial = std::make_shared<PhongMaterial>();
		wheelMaterial->mDiffuse = std::make_shared<Texture>("assets/textures/grass.jpg", 0, GL_SRGB_ALPHA);
		auto wheel = std::make_shared<Mesh>(wheelGeometry, wheelMaterial);
		wheel->setAngleZ(90.f);
		scene->addChild(wheel);
	}
}

void cleanupVehicles()
{
	gVehicle.destroy();
}

void cleanupGroundPlane()
{
	PX_RELEASE(gGroundPlane);
}

void cleanupPhysX()
{
	PxCloseVehicleExtension();
	PX_RELEASE(gMaterial);
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		PX_RELEASE(gPvd);
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}

void releasePhysics()
{
	cleanupVehicles();
	cleanupGroundPlane();
	cleanupPhysX();
}

void LoadBaseParams(BaseVehicleParams& baseParams)
{
	PxMemSet(&baseParams.axleDescription, 0xff, sizeof(baseParams.axleDescription));
	baseParams.axleDescription.setToDefault();
	PxU32 axleWheelIdF[PxVehicleLimits::eMAX_NB_WHEELS] = { 0, 1 };
	PxU32 axleWheelIdR[PxVehicleLimits::eMAX_NB_WHEELS] = { 2, 3 };
	baseParams.axleDescription.addAxle(2, axleWheelIdF); //Front
	baseParams.axleDescription.addAxle(2, axleWheelIdR); //Rear


	PxMemSet(&baseParams.frame, 0xff, sizeof(baseParams.frame));
	baseParams.frame.lngAxis = static_cast<PxVehicleAxes::Enum>(4);
	baseParams.frame.latAxis = static_cast<PxVehicleAxes::Enum>(0);
	baseParams.frame.vrtAxis = static_cast<PxVehicleAxes::Enum>(2);

	PxMemSet(&baseParams.scale, 0xff, sizeof(baseParams.scale));
	baseParams.scale.scale = 1.0;

	//////////////////////////////
	//Read the rigid body params
	/////////////////////////////

	PxMemSet(&baseParams.rigidBodyParams, 0xff, sizeof(baseParams.rigidBodyParams));
	baseParams.rigidBodyParams.mass = 2014.4000244140625;
	baseParams.rigidBodyParams.moi.x = 3200.0; // moi是转动惯量
	baseParams.rigidBodyParams.moi.y = 3414.0;
	baseParams.rigidBodyParams.moi.z = 750.0;

	//////////////////////////////
	//Read the suspension state calculation params.
	//////////////////////////////

	PxMemSet(&baseParams.suspensionStateCalculationParams, 0xff, sizeof(baseParams.suspensionStateCalculationParams));
	baseParams.suspensionStateCalculationParams.suspensionJounceCalculationType
		= static_cast<PxVehicleSuspensionJounceCalculationType::Enum>(1);
	baseParams.suspensionStateCalculationParams.limitSuspensionExpansionVelocity = false;
	///////////////////////////////
	//Read the command responses
	///////////////////////////////

	PxMemSet(&baseParams.brakeResponseParams[0], 0xff, sizeof(baseParams.brakeResponseParams[0]));

	baseParams.brakeResponseParams[0].maxResponse = static_cast<PxReal>(1875.0);
	for (PxU32 i = 0; i < 4; i++)
	{
		baseParams.brakeResponseParams[0].wheelResponseMultipliers[i] = static_cast<PxReal>(1.0);
	}
	//Nonlinear response is not mandatory.
	baseParams.brakeResponseParams[0].nonlinearResponse.clear();

	PxMemSet(&baseParams.brakeResponseParams[1], 0xff, sizeof(baseParams.brakeResponseParams[1]));
	baseParams.brakeResponseParams[1].maxResponse = static_cast<PxReal>(0.0);
	baseParams.brakeResponseParams[1].wheelResponseMultipliers[0] = static_cast<PxReal>(0.0);
	baseParams.brakeResponseParams[1].wheelResponseMultipliers[1] = static_cast<PxReal>(0.0);
	baseParams.brakeResponseParams[1].wheelResponseMultipliers[2] = static_cast<PxReal>(1.0);
	baseParams.brakeResponseParams[1].wheelResponseMultipliers[3] = static_cast<PxReal>(1.0);
	baseParams.brakeResponseParams[1].nonlinearResponse.clear();


	PxMemSet(&baseParams.steerResponseParams, 0xff, sizeof(baseParams.steerResponseParams));
	baseParams.steerResponseParams.maxResponse = static_cast<PxReal>(M_PI);
	baseParams.steerResponseParams.wheelResponseMultipliers[0] = static_cast<PxReal>(1.0);
	baseParams.steerResponseParams.wheelResponseMultipliers[1] = static_cast<PxReal>(1.0);
	baseParams.steerResponseParams.wheelResponseMultipliers[2] = static_cast<PxReal>(0.0);
	baseParams.steerResponseParams.wheelResponseMultipliers[3] = static_cast<PxReal>(0.0);
	baseParams.steerResponseParams.nonlinearResponse.clear();

	PxMemSet(&baseParams.ackermannParams, 0xff, sizeof(baseParams.ackermannParams));

	baseParams.ackermannParams[0].wheelBase = static_cast<PxReal>(2.863219976425171);

	baseParams.ackermannParams[0].trackWidth = static_cast<PxReal>(1.5510799884796143);

	baseParams.ackermannParams[0].strength = static_cast<PxReal>(1.0); // strength是刚度
	baseParams.ackermannParams[0].wheelIds[0] = 0;
	baseParams.ackermannParams[0].wheelIds[1] = 1;

	///////////////////////////////////
	//Read the suspension params
	///////////////////////////////////

	PxMemSet(baseParams.suspensionParams, 0xff, sizeof(baseParams.suspensionParams));
	
	baseParams.suspensionParams[0].suspensionAttachment.p = PxVec3(-0.7952629923820496, -0.10795299708843231, 1.269219994544983); // 悬挂点
	baseParams.suspensionParams[0].suspensionAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);
	baseParams.suspensionParams[0].suspensionTravelDir = PxVec3(0.0, -1.0, 0.0);
	baseParams.suspensionParams[0].suspensionTravelDist = 0.221110999584198; // 悬挂长度
	baseParams.suspensionParams[0].wheelAttachment.p = PxVec3(0.0, 0.0, 0.0);
	baseParams.suspensionParams[0].wheelAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);

	baseParams.suspensionParams[1].suspensionAttachment.p = PxVec3(0.7952629923820496, -0.10795299708843231, 1.269219994544983);
	baseParams.suspensionParams[1].suspensionAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);
	baseParams.suspensionParams[1].suspensionTravelDir = PxVec3(0.0, -1.0, 0.0);
	baseParams.suspensionParams[1].suspensionTravelDist = 0.221110999584198;
	baseParams.suspensionParams[1].wheelAttachment.p = PxVec3(0.0, 0.0, 0.0);
	baseParams.suspensionParams[1].wheelAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);

	baseParams.suspensionParams[2].suspensionAttachment.p = PxVec3(-0.7952629923820496, -0.10795299708843231, -1.593999981880188);
	baseParams.suspensionParams[2].suspensionAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);
	baseParams.suspensionParams[2].suspensionTravelDir = PxVec3(0.0, -1.0, 0.0);
	baseParams.suspensionParams[2].suspensionTravelDist = 0.221110999584198;
	baseParams.suspensionParams[2].wheelAttachment.p = PxVec3(0.0, 0.0, 0.0);
	baseParams.suspensionParams[2].wheelAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);

	baseParams.suspensionParams[3].suspensionAttachment.p = PxVec3(0.7952629923820496, -0.10795299708843231, -1.593999981880188);
	baseParams.suspensionParams[3].suspensionAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);
	baseParams.suspensionParams[3].suspensionTravelDir = PxVec3(0.0, -1.0, 0.0);
	baseParams.suspensionParams[3].suspensionTravelDist = 0.221110999584198;
	baseParams.suspensionParams[3].wheelAttachment.p = PxVec3(0.0, 0.0, 0.0);
	baseParams.suspensionParams[3].wheelAttachment.q = PxQuat(0.0, 0.0, 0.0, 1.0);


	PxMemSet(baseParams.suspensionComplianceParams, 0x00, sizeof(baseParams.suspensionComplianceParams));

	baseParams.suspensionComplianceParams[0].wheelToeAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[0].wheelCamberAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[0].suspForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));
	baseParams.suspensionComplianceParams[0].tireForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));

	baseParams.suspensionComplianceParams[1].wheelToeAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[1].wheelCamberAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[1].suspForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));
	baseParams.suspensionComplianceParams[1].tireForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));

	baseParams.suspensionComplianceParams[2].wheelToeAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[2].wheelCamberAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[2].suspForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));
	baseParams.suspensionComplianceParams[2].tireForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));

	baseParams.suspensionComplianceParams[3].wheelToeAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[3].wheelCamberAngle.addPair(0.0f, 0.0f);
	baseParams.suspensionComplianceParams[3].suspForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));
	baseParams.suspensionComplianceParams[3].tireForceAppPoint.addPair(0.0f, PxVec3(0.0f, 0.0f, -0.11204999685287476f));


	PxMemSet(baseParams.suspensionForceParams, 0xff, sizeof(baseParams.suspensionForceParams));

	baseParams.suspensionForceParams[0].damping = 8528.1201171875f;
	baseParams.suspensionForceParams[0].stiffness = 32833.30078125f;
	baseParams.suspensionForceParams[0].sprungMass = 553.7739868164063f;

	baseParams.suspensionForceParams[1].damping = 8742.1904296875f;
	baseParams.suspensionForceParams[1].stiffness = 33657.3984375f;
	baseParams.suspensionForceParams[1].sprungMass = 567.6749877929688f;

	baseParams.suspensionForceParams[2].damping = 6765.97021484375f;
	baseParams.suspensionForceParams[2].stiffness = 26049.0f;
	baseParams.suspensionForceParams[2].sprungMass = 439.3489990234375f;

	baseParams.suspensionForceParams[3].damping = 6985.47998046875f;
	baseParams.suspensionForceParams[3].stiffness = 26894.099609375f;
	baseParams.suspensionForceParams[3].sprungMass = 453.6029968261719f;

	///////////////////////////////////
	//Read the tire params
	///////////////////////////////////

	PxMemSet(baseParams.tireForceParams, 0xff, sizeof(baseParams.tireForceParams));

	baseParams.tireForceParams[0].longStiff = 24525.0f;
	baseParams.tireForceParams[0].latStiffX = 0.009999999776482582f;
	baseParams.tireForceParams[0].latStiffY = 118699.637252138f;
	baseParams.tireForceParams[0].camberStiff = 0.0;
	baseParams.tireForceParams[0].restLoad = 5628.72314453125f;
	baseParams.tireForceParams[0].frictionVsSlip[0][0] = 0.0f;
	baseParams.tireForceParams[0].frictionVsSlip[0][1] = 1.0f;
	baseParams.tireForceParams[0].frictionVsSlip[1][0] = 0.10000000149011612f;
	baseParams.tireForceParams[0].frictionVsSlip[1][1] = 1.0f;
	baseParams.tireForceParams[0].frictionVsSlip[2][0] = 1.0f;
	baseParams.tireForceParams[0].frictionVsSlip[2][1] = 1.0f;
	baseParams.tireForceParams[0].loadFilter[0][0] = 0.0f;
	baseParams.tireForceParams[0].loadFilter[0][1] = 0.23080000281333924f;
	baseParams.tireForceParams[0].loadFilter[1][0] = 3.0f;
	baseParams.tireForceParams[0].loadFilter[1][1] = 3.0f;

	baseParams.tireForceParams[1].longStiff = 24525.0f;
	baseParams.tireForceParams[1].latStiffX = 0.009999999776482582f;
	baseParams.tireForceParams[1].latStiffY = 118699.637252138f;
	baseParams.tireForceParams[1].camberStiff = 0.0;
	baseParams.tireForceParams[1].restLoad = 5628.72314453125f;
	baseParams.tireForceParams[1].frictionVsSlip[0][0] = 0.0f;
	baseParams.tireForceParams[1].frictionVsSlip[0][1] = 1.0f;
	baseParams.tireForceParams[1].frictionVsSlip[1][0] = 0.10000000149011612f;
	baseParams.tireForceParams[1].frictionVsSlip[1][1] = 1.0f;
	baseParams.tireForceParams[1].frictionVsSlip[2][0] = 1.0f;
	baseParams.tireForceParams[1].frictionVsSlip[2][1] = 1.0f;
	baseParams.tireForceParams[1].loadFilter[0][0] = 0.0f;
	baseParams.tireForceParams[1].loadFilter[0][1] = 0.23080000281333924f;
	baseParams.tireForceParams[1].loadFilter[1][0] = 3.0f;
	baseParams.tireForceParams[1].loadFilter[1][1] = 3.0f;

	baseParams.tireForceParams[2].longStiff = 24525.0f;
	baseParams.tireForceParams[2].latStiffX = 0.009999999776482582f;
	baseParams.tireForceParams[2].latStiffY = 143930.84033118f;
	baseParams.tireForceParams[2].camberStiff = 0.0;
	baseParams.tireForceParams[2].restLoad = 4604.3134765625f;
	baseParams.tireForceParams[2].frictionVsSlip[0][0] = 0.0f;
	baseParams.tireForceParams[2].frictionVsSlip[0][1] = 1.0f;
	baseParams.tireForceParams[2].frictionVsSlip[1][0] = 0.10000000149011612f;
	baseParams.tireForceParams[2].frictionVsSlip[1][1] = 1.0f;
	baseParams.tireForceParams[2].frictionVsSlip[2][0] = 1.0f;
	baseParams.tireForceParams[2].frictionVsSlip[2][1] = 1.0f;
	baseParams.tireForceParams[2].loadFilter[0][0] = 0.0f;
	baseParams.tireForceParams[2].loadFilter[0][1] = 0.23080000281333924f;
	baseParams.tireForceParams[2].loadFilter[1][0] = 3.0f;
	baseParams.tireForceParams[2].loadFilter[1][1] = 3.0f;

	baseParams.tireForceParams[3].longStiff = 24525.0f;
	baseParams.tireForceParams[3].latStiffX = 0.009999999776482582f;
	baseParams.tireForceParams[3].latStiffY = 143930.84033118f;
	baseParams.tireForceParams[3].camberStiff = 0.0;
	baseParams.tireForceParams[3].restLoad = 4604.3134765625f;
	baseParams.tireForceParams[3].frictionVsSlip[0][0] = 0.0f;
	baseParams.tireForceParams[3].frictionVsSlip[0][1] = 1.0f;
	baseParams.tireForceParams[3].frictionVsSlip[1][0] = 0.10000000149011612f;
	baseParams.tireForceParams[3].frictionVsSlip[1][1] = 1.0f;
	baseParams.tireForceParams[3].frictionVsSlip[2][0] = 1.0f;
	baseParams.tireForceParams[3].frictionVsSlip[2][1] = 1.0f;
	baseParams.tireForceParams[3].loadFilter[0][0] = 0.0f;
	baseParams.tireForceParams[3].loadFilter[0][1] = 0.23080000281333924f;
	baseParams.tireForceParams[3].loadFilter[1][0] = 3.0f;
	baseParams.tireForceParams[3].loadFilter[1][1] = 3.0f;

	//////////////////////////
	//Read the wheel params
	//////////////////////////

	PxMemSet(baseParams.wheelParams, 0xff, sizeof(baseParams.wheelParams));
	for (PxU32 i = 0; i < 4; ++i)
	{
		baseParams.wheelParams[i].halfWidth = 0.225f;
		baseParams.wheelParams[i].radius = 0.3432520031929016f;
		baseParams.wheelParams[i].mass = 20.0f;
		baseParams.wheelParams[i].moi = 1.1716899871826172f;
		baseParams.wheelParams[i].dampingRate = 0.25f;
	}
}

void LoadEngineDrivertrainParams(EngineDrivetrainParams& engineDrivetrainParams)
{
	float defaultRatio = 0.65f;
	for (int i = 0; i < 7; ++i) engineDrivetrainParams.autoboxParams.upRatios[i] = defaultRatio;
	engineDrivetrainParams.autoboxParams.upRatios[1] = 0.15f;

	for (int i = 0; i < 7; ++i) engineDrivetrainParams.autoboxParams.downRatios[i] = 0.5f;

	engineDrivetrainParams.autoboxParams.latency = 2.0f;
	engineDrivetrainParams.clutchCommandResponseParams.maxResponse = 10.0f;
	engineDrivetrainParams.engineParams.torqueCurve.addPair(0.0f, 1.0f);
	engineDrivetrainParams.engineParams.torqueCurve.addPair(0.33f, 1.0f);
	engineDrivetrainParams.engineParams.torqueCurve.addPair(1.0f, 1.0f);

	engineDrivetrainParams.engineParams.moi = 1.0f;
	engineDrivetrainParams.engineParams.peakTorque = 500.0f; // 最大扭矩
	engineDrivetrainParams.engineParams.idleOmega = 0.0f;
	engineDrivetrainParams.engineParams.maxOmega = 600.0f; // 最大转速
	engineDrivetrainParams.engineParams.dampingRateFullThrottle = 0.15f;
	engineDrivetrainParams.engineParams.dampingRateZeroThrottleClutchEngaged = 2.0f;
	engineDrivetrainParams.engineParams.dampingRateZeroThrottleClutchDisengaged = 0.35f;
	engineDrivetrainParams.gearBoxParams.neutralGear = 1;

	engineDrivetrainParams.gearBoxParams.finalRatio = 4.0f;
	engineDrivetrainParams.gearBoxParams.switchTime = 0.5f;
	engineDrivetrainParams.gearBoxParams.ratios[0] = -4.0f;
	engineDrivetrainParams.gearBoxParams.ratios[1] = 0.0f;
	engineDrivetrainParams.gearBoxParams.ratios[2] = 4.0f;
	engineDrivetrainParams.gearBoxParams.ratios[3] = 2.0f;
	engineDrivetrainParams.gearBoxParams.ratios[4] = 1.5f;
	engineDrivetrainParams.gearBoxParams.ratios[5] = 1.1f;
	engineDrivetrainParams.gearBoxParams.ratios[6] = 1.0f;
	engineDrivetrainParams.gearBoxParams.nbRatios = 7;
	for (int i = 0; i < 4; ++i)
	{
		engineDrivetrainParams.multiWheelDifferentialParams.torqueRatios[i] = 0.25f;
		engineDrivetrainParams.multiWheelDifferentialParams.aveWheelSpeedRatios[i] = 0.25f;
	}
	for (int i = 0; i < 4; ++i)
	{
		engineDrivetrainParams.fourWheelDifferentialParams.torqueRatios[i] = 0.25f;
		engineDrivetrainParams.fourWheelDifferentialParams.aveWheelSpeedRatios[i] = 0.25f;
	}
	engineDrivetrainParams.fourWheelDifferentialParams.frontWheelIds[0] = 0;
	engineDrivetrainParams.fourWheelDifferentialParams.frontWheelIds[1] = 1;
	engineDrivetrainParams.fourWheelDifferentialParams.rearWheelIds[0] = 2;
	engineDrivetrainParams.fourWheelDifferentialParams.rearWheelIds[1] = 3;
	engineDrivetrainParams.fourWheelDifferentialParams.centerBias = 1.3f;
	engineDrivetrainParams.fourWheelDifferentialParams.centerTarget = 1.29f;
	engineDrivetrainParams.fourWheelDifferentialParams.frontBias = 1.3f;
	engineDrivetrainParams.fourWheelDifferentialParams.frontTarget = 1.29f;
	engineDrivetrainParams.fourWheelDifferentialParams.rearBias = 1.3f;
	engineDrivetrainParams.fourWheelDifferentialParams.rearTarget = 1.29f;
	engineDrivetrainParams.fourWheelDifferentialParams.rate = 10.0f;
	for (int i = 0; i < 4; ++i)
	{
		engineDrivetrainParams.tankDifferentialParams.torqueRatios[i] = 0.25f;
		engineDrivetrainParams.tankDifferentialParams.aveWheelSpeedRatios[i] = 0.25f;
	}
	PxU32 wheelId1[PxVehicleLimits::eMAX_NB_WHEELS] = { 0, 2 };
	PxU32 wheelId2[PxVehicleLimits::eMAX_NB_WHEELS] = { 1, 3 };
	PxU32 nbWheels = 2;
	engineDrivetrainParams.tankDifferentialParams.addTankTrack(nbWheels, wheelId1, 0);
	engineDrivetrainParams.tankDifferentialParams.addTankTrack(nbWheels, wheelId2, 1);
	engineDrivetrainParams.clutchParams.accuracyMode = static_cast<vehicle2::PxVehicleClutchAccuracyMode::Enum>(1);
	engineDrivetrainParams.clutchParams.estimateIterations = static_cast<PxU32>(5);

}

#endif // PHYSX_TOOL_H