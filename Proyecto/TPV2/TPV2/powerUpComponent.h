#pragma once
#include "Component.h"
#include "ecs.h"
using namespace std;
class powerUpComponent :public Component
{
private:
	int id2 = 0;
	float timer = 0;
public:
	constexpr static cmpId_type id = POWERUPCOMPONENT_H;

	powerUpComponent(int id1) { id2 = id1; };
	~powerUpComponent() {};

	inline int getId() { return id2; }
	inline void setId(int newId) { id2 = newId; }

	inline void setTimer(float newTimer) { timer = newTimer; }

	inline float getTimer() { return timer; }
};

