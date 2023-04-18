#pragma once
#ifndef GENERATIONS_H_
#define GENERATIONS_H_
#include "Component.h"

struct Generations : public Component // Component that contains the generations data of each asteroid
{
private:
	int generations;
public:
	constexpr static cmpId_type id = GENERATIONS_H;

	Generations(int GenInicial) : Component(), generations(GenInicial) {};
	inline int getGenerations() { return generations; }
	inline void setGenerations(int newGen) { generations = newGen; }
};

#endif
