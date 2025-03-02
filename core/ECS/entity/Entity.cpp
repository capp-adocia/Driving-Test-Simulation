/*****************************************************************//**
 * \file   Entity.cpp
 * \brief  实体
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#include "Entity.h"
#include "../../world/world.h"


Entity::Entity(const entt::entity id, World* world)
	: m_EntityId(id)
	, m_World(world)
{
}

entt::registry& Entity::registry() const
{
	return world().registry();
}