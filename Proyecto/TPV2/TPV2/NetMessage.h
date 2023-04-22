#pragma once
#include "ecs.h"
struct NetMessage
{
	netMsgId_type id;
	//Actualizar bala y nave
};

struct ShootMessage :public NetMessage
{
	float x;
	float y;
	float velX;
	float velY;
	float rot;
	float width;
	float height;
};

struct FighterPosMessage :public NetMessage
{
	float x;
	float y;
	float velX;
	float velY;
	float rot;
	float width;
	float height;
};

struct GameOverMessage :public NetMessage
{
	bool fighter1Winner;
};

struct InitialMessage:public NetMessage
{
	int n;
};
