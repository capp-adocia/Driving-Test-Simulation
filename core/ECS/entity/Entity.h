/*****************************************************************//**
 * \file   Entity.h
 * \brief  实体
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   February 2025
 *********************************************************************/
#ifndef ENTITY_H
#define ENTITY_H
#include "../../../util/common/opengl.h"
#include "../component/Component.h"
class World;

using component::TagComponent;
using component::TransformComponent;

class Entity
{
public:
	Entity() = default;
	Entity(const entt::entity id, World* world);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args) {
		return registry().emplace<T>(m_EntityId, std::forward<Args>(args)...);
	}

	template<typename T>
	void RemoveComponent() {
		registry().remove<T>(m_EntityId);
	}

	template<typename T>
	T& GetComponent() {
		return registry().get<T>(m_EntityId);
	}

	template<typename T>
	bool HasComponent() const {
		return registry().any_of<T>(m_EntityId);
	}

	inline TransformComponent& GetTransform() { return GetComponent<TransformComponent>(); }
	inline TagComponent& GetTag() { return GetComponent<TagComponent>(); }

	inline operator bool() const { return m_EntityId != entt::null; }
	inline operator entt::entity() const { return m_EntityId; }
	inline operator entt::id_type() const { return static_cast<entt::id_type>(m_EntityId); }

	inline bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_World == other.m_World; }
	inline bool operator!=(const Entity& other) const { return !(*this == other); }

	inline entt::entity handle() const { return m_EntityId; }
	inline entt::id_type id() const { return static_cast<entt::id_type>(m_EntityId); }
	World& world() const { return *m_World; }
	entt::registry& registry() const; /* return world().registry() */ ;
private:
	entt::entity m_EntityId;
	World* m_World;
};

#endif // !ENTITY_H
