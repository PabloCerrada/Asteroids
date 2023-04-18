#pragma once
#include "Manager.h"
#include "src/sdlutils/Font.h"
#include "RenderSystem.h"
#include "GameCtrlSystem.h"
class Game;
class MainMenuState :public Manager
{
private:
	Game* game;
	RenderSystem* renderSystem;
	GameCtrlSystem* gameCtrlSystem;
public:
	MainMenuState(Game* game);
	virtual ~MainMenuState();
	virtual void update();
};

