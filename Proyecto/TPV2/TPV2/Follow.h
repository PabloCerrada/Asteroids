#pragma once
#ifndef FOLLOW_H_
#define FOLLOW_H_
#include "Component.h"
#include "ecs.h"

struct Follow : public Component // This component exists to make the difference between both types of asteroids
{
private:

public:
	constexpr static cmpId_type id = FOLLOW_H;
	Follow() {}
	virtual ~Follow() {}
};

#endif
