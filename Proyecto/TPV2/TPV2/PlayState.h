#pragma once
#include "FighterSystem.h"
#include "RenderSystem.h"
#include "GameCtrlSystem.h"
#include "BulletSystem.h"
#include "AsteroidsSystem.h"
#include "CollisionSystem.h"

class Game;
class PlayState : public Manager
{
private:
	FighterSystem* fighterSystem;
	RenderSystem* renderSystem;
	GameCtrlSystem* gameCrtlSystem;
	BulletSystem* bulletSystem;
	AsteroidsSystem* asteroidSystem;
	CollisionSystem* collisionSystem;
	bool gameover = false, win = false; // control booleans
	Game* game;
public:
	PlayState(Game* game1);
	virtual ~PlayState();
	virtual void update();
	inline void setGameover(bool newValue) { gameover = newValue; }
	inline Game* getGame() { return game; }
	void MainMenu();
};
