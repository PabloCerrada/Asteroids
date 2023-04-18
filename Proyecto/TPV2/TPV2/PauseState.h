#pragma once
#include "Manager.h"
#include "GameCtrlSystem.h"
#include "RenderSystem.h"
class Game;
class PauseState :public Manager {
private:
	Game* game;
	GameCtrlSystem* gameControlSystem;
	RenderSystem* renderSystem;
public:
	PauseState(Game* game);
	virtual ~PauseState();
	virtual void update();
};


