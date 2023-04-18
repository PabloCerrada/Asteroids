#include "PauseState.h"
#include "Game.h"
PauseState::PauseState(Game* game) {
	this->game = game;
	id = "PauseState";
	renderSystem = addSystem<RenderSystem>();
	gameControlSystem = addSystem<GameCtrlSystem>(game);
}

PauseState::~PauseState()
{
	removeSystem<RenderSystem>();
	removeSystem<GameCtrlSystem>();
}

void PauseState::update()
{
	renderSystem->update();
	gameControlSystem->update();
}
