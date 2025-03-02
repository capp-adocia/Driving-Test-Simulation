/*****************************************************************//**
 * \file   world.h
 * \brief  处理、更新世界信息
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#ifndef WORLD_H
#define WORLD_H
#include "../../util/common/opengl.h"
#include "../../util/common/physx.h"
#include "../ecs/entity/Entity.h"

class PhysicsSystem;
class RenderSystem;

class World
{
public:
	explicit World();
	~World();

	Entity CreateEntity(const std::string& name = "Entity");
	void DestroyEntity(const entt::entity entityId);
	/* registry给Entity类使用 */
	inline entt::registry& registry() { return m_EntityRegistry; }
	inline bool IsPaused() const { return m_Paused; }
	inline void SetPause(bool paused) { m_Paused = paused; }
	inline void SetPauseSteps(int steps) { m_PausedStepFrames = steps; }
	void LoadWorld();
	void OnTick(float dt);
private:
	entt::registry m_EntityRegistry;
	bool m_Paused;
	int  m_PausedStepFrames;
	std::unique_ptr<PhysicsSystem> m_Physics;
	std::unique_ptr<RenderSystem> m_Render;
};
#endif // !WORLD_H