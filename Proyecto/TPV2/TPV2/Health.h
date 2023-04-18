#pragma once
#ifndef HEALTH_H_
#define HEALTH_H_
#include "Component.h"
#include "ecs.h"

struct Health : public Component
{
private:
	int maxLives;
	int lives;

public:
	constexpr static cmpId_type id = HEALTH_H;
	Health(int maxLifes_);
	~Health(){}
	inline void quitLife() { --lives; }
	inline void resetLifes() { lives = maxLives; }
	inline int getLifes() { return lives; }
};
#endif

