#include "Game.h"

Game::Game() {
	// We first initialize SDL
	SDLUtils::init("Hola", WIN_WIDTH, WIN_HEIGHT, "resources/config/sdlutilsdemo.resources.json");
	renderer = SDLUtils::instance()->renderer();
	window = SDLUtils::instance()->window();


	GameStateMachine::instance()->pushState(new MainMenuState(this));
}
Game::~Game() {
	GameStateMachine::instance()->~GameStateMachine();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run() {
	if (SDL_Init(0) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}
	if (SDLNet_Init() == -1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}

	cout << "1 para server\n2 para cliente" << endl;
	string a;
	cin >> a;

	if (a == "1") server(9999);
	else {
		// Pregunta por la IP
		cout << "IP: ";
		char host[1024];
		cin >> host;
		client(host, 9999);
	}

	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();
	while (!end) {
		frameTime = SDL_GetTicks() - startTime;
		if (frameTime >= FRAME_RATE) {
			update();
			refresh();
			startTime = SDL_GetTicks();
		}
	}
}

void Game::server(int port) {
	//IPaddress ip;
	//if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) {
	//	throw "ERROR AL ESTABLECER LA CONEXION CON EL SERVIDOR SIENDO SERVER";
	//}
	//TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	//if (!masterSocket) {
	//	throw "ERROR AL ABRIR EL SERVER SIENDO SERVER";
	//}

	//TCPsocket client = SDLNet_TCP_Accept(masterSocket);

	////int result = SDLNet_TCP_Recv(conn, data, n);

	//SDLNet_TCP_Close(masterSocket);
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, nullptr, port) < 0) { 
		throw "ERROR AL ESTABLECER LA CONEXION CON EL SERVIDOR SIENDO SERVER";
	}
	TCPsocket masterSocket = SDLNet_TCP_Open(&ip);
	if (!masterSocket) { 
		throw "ERROR AL ABRIR EL SERVER SIENDO SERVER";
	}
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketSet, masterSocket);
	while (true) {
		if (SDLNet_CheckSockets(socketSet, SDL_MAX_UINT32) > 0) {
			// TODO I: process connection request on masterSocket
			char buffer[256];
			int result = 0;
			// ...
			if (SDLNet_SocketReady(masterSocket)) {
				TCPsocket client = SDLNet_TCP_Accept(masterSocket);
				result = SDLNet_TCP_Recv(client, buffer, 255);
				if (result > 0) {
					cout << "Client says: " << buffer << endl;
					SDLNet_TCP_Send(client, "Received!", 10);
				}
				SDLNet_TCP_Close(client);
			}
		}
	}
	SDLNet_FreeSocketSet(socketSet);
	SDLNet_TCP_Close(masterSocket);
}

void Game::client(char* host, int port) {
	//IPaddress ip;
	//if (SDLNet_ResolveHost(&ip, host, port) < 0) {
	//	throw "ERROR AL ESTABLECER LA CONEXION CON EL SERVIDOR SIENDO CLIENTE";
	//}
	//TCPsocket conn = SDLNet_TCP_Open(&ip);
	//if (!conn) {
	//	throw "ERROR AL ABRIR EL SERVER SIENDO CLIENTE";
	//}

	////int result = SDLNet_TCP_Send(conn, data, n);

	//SDLNet_TCP_Close(conn);

	char buffer[256];
	int result = 0;
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, host, port) < 0) { 
		throw "ERROR AL ESTABLECER LA CONEXION CON EL SERVIDOR SIENDO CLIENTE";
	}
	TCPsocket conn = SDLNet_TCP_Open(&ip);
	if (!conn) { 
		throw "ERROR AL ABRIR EL SERVER SIENDO CLIENTE";
	}
	// TODO: SEND MSG AND WAIT FOR RESPONSE
	while (true) // 192.168.1.55
	{
		cout << "Enter a message: ";
		cin.getline(buffer, 255);
		int size = strlen(buffer) + 1;
		result = SDLNet_TCP_Send(conn, buffer, size);

		if (result != size) {
			throw "ERROR: buffer distinto de tamano resultado";
		}
		result = SDLNet_TCP_Recv(conn, buffer, 255);

		if (result < 0) throw "ERROR AL RECIBIR RESULTADO";
		else if (result == 0) cout << "server closed";
		else cout << buffer << endl;

		SDLNet_TCP_Close(conn);

	} // hola
}

void Game::update()
{
	GameStateMachine::instance()->currentState()->update();
}

void Game::refresh()
{
	GameStateMachine::instance()->currentState()->refresh();
}

void Game::setExit() {
	
	end = !end;
}

void Game::playFunction(Game* game) {
	
	GameStateMachine::instance()->changeState(new PlayState(game));
}

void Game::exitFunction(Game* game) 
{
	GameStateMachine::instance()->clearState();
	game->setExit();
}

void Game::pauseFunction(Game* game)
{
	GameStateMachine::instance()->pushState(new PauseState(game));
}

void Game::returnToGame(Game* game)
{
	GameStateMachine::instance()->popState();
}

void Game::returnToMainMenu(Game* game)
{
	GameStateMachine::instance()->changeState(new MainMenuState(game));
}