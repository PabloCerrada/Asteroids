#include "PlayState.h"
#include "Game.h"

PlayState::PlayState(Game* game1) {
	game = game1;
	id = "PlayState";
	sys_[_sys_ASTEROIDS] = nullptr;
	sys_[_sys_COLLISIONS] = nullptr;
	sys_[_sys_RENDER] = nullptr;
	sys_[_sys_FIGHTER] = nullptr;
	sys_[_sys_GAMECTRL] = nullptr;
	sys_[_sys_BULLET] = nullptr;
	sys_[_sys_NET] = nullptr;
	sys_[_sys_BOMB] = nullptr;
	sys_[_sys_POWERUP] = nullptr;

	bulletSystem = addSystem<BulletSystem>();
	fighterSystem = addSystem<FighterSystem>();
	renderSystem = addSystem<RenderSystem>();
	asteroidSystem = addSystem<AsteroidsSystem>();
	collisionSystem = addSystem<CollisionSystem>();
	bombSystem = addSystem<BombSystem>();
	powerUp = addSystem<powerUpSystem>();
	gameCrtlSystem = addSystem<GameCtrlSystem>(game);


	Message m;
	m.id = _msg_STARTGAME;
	send(m);
}
PlayState::~PlayState() 
{
	removeSystem<BulletSystem>();
	removeSystem<FighterSystem>();
	removeSystem<RenderSystem>();
	removeSystem<AsteroidsSystem>();
	removeSystem<CollisionSystem>();
	removeSystem<GameCtrlSystem>();
	removeSystem<BombSystem>();
	removeSystem<powerUpSystem>();
}
void PlayState::update() {
	if (!gameover) {
		fighterSystem->update();
		renderSystem->update();
		bulletSystem->update();
		asteroidSystem->update();
		bombSystem->update();
		collisionSystem->update();
		powerUp->update();
		gameCrtlSystem->update();
	}
}

void PlayState::MainMenu() {
	game->returnToMainMenu(game);
}