#include "MultiplayerState.h"
#include "Game.h"

MultiplayerState::MultiplayerState(Game* game1) {
	id = "Multiplayer";

	if (SDLNet_Init() < 0) {
		throw "Error de conexion";
	}

	if (SDL_Init(0) == -1) {
		printf("SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}
	if (SDLNet_Init() == -1) {
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}

	cout << "Pulsa 1 para ser el creador de la partida\nPulsa 2 para unirte a una partida" << endl;
	string a;
	cin >> a;

	if (a == "1") {
		iAmServer = true;
		server(port_);
	}
	else {
		iAmServer = false;
		// Pregunta por la IP
		cout << "IP: ";
		char host[1024];
		cin >> host;
		client(host, port_);
	}

	game = game1;
	sys_[_sys_ASTEROIDS] = nullptr;
	sys_[_sys_COLLISIONS] = nullptr;
	sys_[_sys_RENDER] = nullptr;
	sys_[_sys_FIGHTER] = nullptr;
	sys_[_sys_GAMECTRL] = nullptr;
	sys_[_sys_BULLET] = nullptr;

	bulletSystem = addSystem<BulletSystem>();
	fighterSystem = addSystem<FighterSystem>();
	renderSystem = addSystem<RenderSystem>();
	collisionSystem = addSystem<CollisionSystem>();
	gameCrtlSystem = addSystem<GameCtrlSystem>(game);

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
	/*SDLNet_FreePacket(p); 
	SDLNet_FreeSocketSet(socketSet);*/
	SDLNet_Quit();

}
void MultiplayerState::update() {
	if (!gameover) {
		fighterSystem->update();
		renderSystem->update();
		bulletSystem->update();
		collisionSystem->update();
		gameCrtlSystem->update();
	}
}

void MultiplayerState::MainMenu() {
	game->returnToMainMenu(game);
}

void MultiplayerState::server(int port) {
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

void MultiplayerState::client(char* host, int port) {
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