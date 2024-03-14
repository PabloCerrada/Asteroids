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
		Entity* explosion;
		Entity* asteroid;
	} explosion;

	struct
	{
		int num;
		Entity* powerUp;
	}powerUp;
	struct
	{
		Vector2D pos;
		Vector2D vel;
		double width, height;
		float rotation;
	} bullet;

	struct
	{
		Vector2D pos;
		Vector2D vel;
		double width, height;
		float rotation;
	}bomb;

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