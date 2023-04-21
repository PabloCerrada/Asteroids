#pragma once
#pragma once
#include "FighterSystem.h"
#include "RenderSystem.h"
#include "GameCtrlSystem.h"
#include "BulletSystem.h"
#include "AsteroidsSystem.h"
#include "CollisionSystem.h"
#include "NetSystem.h"
class Game;
class MultiplayerState : public Manager
{
private:
	FighterSystem* fighterSystem;
	RenderSystem* renderSystem;
	GameCtrlSystem* gameCrtlSystem;
	BulletSystem* bulletSystem;
	CollisionSystem* collisionSystem;
	NetSystem* netSystem;
	bool gameover = false, win = false; // control booleans
	Game* game;

	//int port_ = 9999;
	//bool iAmServer;
public:
	MultiplayerState(Game* game1);
	virtual ~MultiplayerState();
	virtual void update();
	inline void setGameover(bool newValue) { gameover = newValue; }
	inline Game* getGame() { return game; }
	void MainMenu();
	/*void server(int port);
	void client(char* host, int port);*/
};
