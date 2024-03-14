#pragma once
#ifndef EXPLOSION_H_
#define EXPLOSION_H_
#include "Component.h"
#include"ecs.h"
class ExplosionTimer :public Component
{
private:
	float deathTimer =0;

public:
	constexpr static cmpId_type id = EXPLOSIONTIMER_H;
	inline void setDeathTimer(float newTimer) { deathTimer = newTimer; }
	inline float getDeathTimer() { return deathTimer; }
	ExplosionTimer() {}
};
#endif

