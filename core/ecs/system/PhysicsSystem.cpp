/*****************************************************************//**
 * \file   PhysicsSystem.cpp
 * \brief  物理模拟
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "PhysicsSystem.h"
//#include "../../physics/physx_tool.h"

using namespace component;

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Init()
{

}

void PhysicsSystem::OnTick(float dt, entt::registry& registry)
{
	auto& view = registry.view<TagComponent>();
    for (entt::entity entity : view) {
        auto& tag = view.get<TagComponent>(entity);

        if (!registry.all_of<PhysicsComponent>(entity)) // all_of可以用来检查是否有这个组件
        {
            LOG_DEBUG("OK!");
        }
    }
}

PhysicsSystem::~PhysicsSystem()
{
}