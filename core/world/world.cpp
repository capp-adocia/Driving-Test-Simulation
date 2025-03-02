/*****************************************************************//**
 * \file   world.cpp
 * \brief  处理、更新世界信息
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "world.h"
#include "../ecs/system/RenderSystem.h"
#include "../ecs/system/PhysicsSystem.h"

World::World()
	: m_EntityRegistry()
	, m_Paused(false)
	, m_PausedStepFrames(0)
	, m_Physics(nullptr)
	, m_Render(nullptr)
{
}

World::~World()
{
}

Entity World::CreateEntity(const std::string& name)
{
	Entity entity{ registry().create(), this };

	// assign: uuid, transform, name.
	registry().emplace<TagComponent>(entity, name);
	registry().emplace<TransformComponent>(entity, glm::mat4(1.0f));
	return entity;
}

void World::DestroyEntity(const entt::entity entityId)
{
	if (!registry().valid(entityId)) {
		LOG_WARN("Entity is not valid");
		return;
	}
	registry().destroy(entityId);

	//if (auto* actor = m_EntityRegistry.try_get<PxActorComponent>(entityId)) {
	//	m_PxScene->removeActor(*actor->pxActor);
	//	actor->pxActor->release();
	//}
}

void World::LoadWorld()
{
	m_Physics = std::make_unique<PhysicsSystem>();
	m_Render = std::make_unique<RenderSystem>();
}

void World::OnTick(float dt)
{
	if (m_Paused && --m_PausedStepFrames < 0)
		return;
	if(!m_Physics || !m_Render)
		LOG_ERROR("Physics or Render is not initialized");

	/* Todo:物理系统 */
	m_Physics->OnTick(1 / 60.0f, m_EntityRegistry);
	/* Todo:渲染系统 */
	m_Render->OnTick(dt, m_EntityRegistry);
	/* Todo:事件系统 */
	{
		//m_EntityDayLight.GetTransform().Transform = glm::rotate(glm::mat4(1.0f), Math::Mod(wi.DayTime * 2.0f, 1.0f) * Math::PI * 2.0f, glm::vec3(0, 0, 1));
		//auto& light = m_EntityDayLight.GetComponent<LightComponent>();
		//float intens = wi.DayTime < 0.5f ? 2.0f * wi.DayTime : 2.0f - 2.0f * wi.DayTime;
		//light.Color = glm::vec3(1.0f) * 0.2f + intens * 0.8f;
	}
}