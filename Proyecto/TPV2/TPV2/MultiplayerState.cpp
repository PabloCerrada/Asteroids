#include "MultiplayerState.h"
#include "Game.h"

MultiplayerState::MultiplayerState(Game* game1) {
	id = "Multiplayer";
	game = game1;
	sys_[_sys_ASTEROIDS] = nullptr;
	sys_[_sys_COLLISIONS] = nullptr;
	sys_[_sys_RENDER] = nullptr;
	sys_[_sys_FIGHTER] = nullptr;
	sys_[_sys_GAMECTRL] = nullptr;
	sys_[_sys_BULLET] = nullptr;
	sys_[_sys_NET] = nullptr;

	bulletSystem = addSystem<BulletSystem>();
	fighterSystem = addSystem<FighterSystem>();
	renderSystem = addSystem<RenderSystem>();
	collisionSystem = addSystem<CollisionSystem>();
	gameCrtlSystem = addSystem<GameCtrlSystem>(game);
	netSystem = addSystem<NetSystem>();

	Message m;
	m.id = _msg_STARTGAME;
	send(m);
}
MultiplayerState::~MultiplayerState()
{
	removeSystem<BulletSystem>();
	removeSystem<FighterSystem>();
	removeSystem<RenderSystem>();
	removeSystem<AsteroidsSystem>();
	removeSystem<CollisionSystem>();
	removeSystem<GameCtrlSystem>();
	SDLNet_Quit();

}
void MultiplayerState::update() {
	if (!gameover) {
		fighterSystem->update();
		renderSystem->update();
		bulletSystem->update();
		collisionSystem->update();
		gameCrtlSystem->update();
		netSystem->update();
	}
}

void MultiplayerState::MainMenu() {
	game->returnToMainMenu(game);
}
