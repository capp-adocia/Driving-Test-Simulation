/*****************************************************************//**
 * \file   PhysicsSystem.cpp
 * \brief  ÎïÀíÄ£Äâ
 *
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#include "PhysicsSystem.h"

using namespace component;

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::Init()
{

}

void PhysicsSystem::OnTick(float dt, entt::registry& registry)
{
	auto& view = registry.view<TagComponent, TransformComponent>();
    for (auto entity : view) {
        auto& tag = view.get<TagComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        printf("%s\n", tag.Name.c_str());
    }
}

PhysicsSystem::~PhysicsSystem()
{
}