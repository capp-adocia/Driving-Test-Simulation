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

class World;

struct TagComponent
{
	std::string Name;
	bool IsEnabled = true;
};
namespace stdx {
	template<typename T>
	T* ptr(const T& ref)
	{
		return reinterpret_cast<T*>(&ref);
	}
	template<typename T, typename V>
	T& cast(V& ptr)
	{
		return *reinterpret_cast<T*>(&ptr);
	}
	template<typename T, typename V>
	const T& cast(const V& ptr)
	{
		return *reinterpret_cast<T*>((void*)&ptr);
	}
}

struct TransformComponent
{
	glm::mat4 Transform;

	const glm::vec3& position() const { return stdx::cast<glm::vec3>(Transform[3]); }

	glm::vec3& position() { return stdx::cast<glm::vec3>(Transform[3]); }

	glm::mat3 basis() const { return glm::mat3(Transform); }

	void set_basis(const glm::mat3& basis) {
		Transform = basis;
	}
	glm::quat quat() const { return glm::quat_cast(basis()); }

};

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity id, World* world)
		: m_EntityId(id)
		, m_World(world) {}
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		T& component = registry().emplace<T>(m_EntityId, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	void RemoveComponent()
	{
		T& c = registry().remove<T>(m_EntityId);
	}

	template<typename T>
	T& GetComponent()
	{
		//STDX_ASSERT(HasComponent<T>(), "Entity does not have the component!");
		return registry().get<T>(m_EntityId);
	}

	template<typename T>
	bool HasComponent() const
	{
		return registry().any_of<T>(m_EntityId);
	}

	TransformComponent& GetTransform() { return GetComponent<TransformComponent>(); }
	TagComponent& GetTag() { return GetComponent<TagComponent>(); }

	operator bool() const { return m_EntityId != entt::null; }
	operator entt::entity() const { return m_EntityId; }
	operator entt::id_type() const { return static_cast<entt::id_type>(m_EntityId); }

	bool operator==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_World == other.m_World; }
	bool operator!=(const Entity& other) const { return !(*this == other); }

	entt::id_type id() const { return static_cast<entt::id_type>(m_EntityId); }
	World& world() { return *m_World; }
	entt::registry& registry() const;
private:

	entt::entity m_EntityId{ entt::null };
	World* m_World = nullptr;
};

#endif // !ENTITY_H
