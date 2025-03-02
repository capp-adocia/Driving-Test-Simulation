/*****************************************************************//**
 * \file   RenderSystem.h
 * \brief  ‰÷»æœµÕ≥
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "System.h"

class RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem() override;
    void Init() override;
    void OnTick(float dt, entt::registry& registry) override;
};
#endif // !RENDERSYSTEM_H