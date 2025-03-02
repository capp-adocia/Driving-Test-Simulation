/*****************************************************************//**
 * \file   System.h
 * \brief  系统基类，处理不同的实体的逻辑
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H
#include "../entity/Entity.h"

class System {
public:
	System() {}
	virtual void Init() = 0;
	virtual void OnTick(float dt, entt::registry& registry) = 0;
	virtual ~System() {}
};
#endif // !SYSTEM_H