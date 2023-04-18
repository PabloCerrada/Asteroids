#include "MainMenuState.h"
#include "Game.h"
MainMenuState::MainMenuState(Game* game) {
	this->game = game;
	id = "MainMenuState";
	sys_[_sys_ASTEROIDS] = nullptr;
	sys_[_sys_COLLISIONS] = nullptr;
	sys_[_sys_RENDER] = nullptr;
	sys_[_sys_FIGHTER] = nullptr;
	sys_[_sys_GAMECTRL] = nullptr;
	sys_[_sys_BULLET] = nullptr;
	renderSystem = addSystem<RenderSystem>();
	gameCtrlSystem = addSystem<GameCtrlSystem>(this->game);
}
MainMenuState::~MainMenuState() 
{
	removeSystem<RenderSystem>();
	removeSystem<GameCtrlSystem>();
}

void MainMenuState::update()
{
	renderSystem->update();
	gameCtrlSystem->update();
}
