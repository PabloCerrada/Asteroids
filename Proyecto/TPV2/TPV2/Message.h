#pragma once
#include "ecs.h"
struct Message
{
	msgId_type id;

	struct
	{
		Entity* bullet;
		Entity* asteroid;
	} colision;

	struct
	{
		Vector2D pos;
		Vector2D vel;
		double width, height;
		float rotation;
	} bullet;

	struct 
	{
		bool deathFighter1;
		bool deathFighter2;
	} endOfRound;

	struct
	{
		bool fighter1Winner;
	} over;
};