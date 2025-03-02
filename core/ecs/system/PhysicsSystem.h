/*****************************************************************//**
 * \file   PhysicsSystem.h
 * \brief  ÎïÀíÄ£Äâ
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    ~PhysicsSystem() override;
    void Init() override;
    void OnTick(float dt, entt::registry& registry) override;
};

#endif // !PHYSICSSYSTEM_H