#include "MultiplayerState.h"
#include "Game.h"

MultiplayerState::MultiplayerState(Game* game1) {
	id = "Multiplayer";

	//if (SDLNet_Init() < 0) {
	//	throw "Error de conexion";
	//}

	//if (SDL_Init(0) == -1) {
	//	printf("SDL_Init: %s\n", SDL_GetError());
	//	exit(1);
	//}
	//if (SDLNet_Init() == -1) {
	//	printf("SDLNet_Init: %s\n", SDLNet_GetError());
	//	exit(2);
	//}

	//cout << "Pulsa 1 para ser el creador de la partida\nPulsa 2 para unirte a una partida" << endl;
	//string a;
	//cin >> a;

	//if (a == "1") {
	//	iAmServer = true;
	//	server(port_);
	//}
	//else {
	//	iAmServer = false;
	//	// Pregunta por la IP
	//	cout << "IP: ";
	//	char host[1024];
	//	cin >> host;
	//	client(host, port_);
	//}

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
