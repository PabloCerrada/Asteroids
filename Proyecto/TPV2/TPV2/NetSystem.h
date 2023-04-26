#pragma once
#include "System.h"
#include "NetMessage.h"
#include <SDL_net.h>
class Game;
class NetSystem : public System
{
private:
	Game* game;
	bool imServer = false, waiting = false;
	int port_ = 9999;
	UDPsocket sock_;
	UDPpacket* p_;
	SDLNet_SocketSet sockSet_;
	NetMessage* message;
	IPaddress ip;
	int MAXPACKETSIZE = 512;


public:
	constexpr static sysId_type id = _sys_NET;
	NetSystem();
	void server(int port);
	void client(char* host, int port);
	void update(); //while recibes mensaje
	inline bool isServer() { return imServer; }
	inline bool getWaiting() { return waiting; }

	void setFighter(Vector2D pos, Vector2D vel, float rotation);
	void createBullet(Vector2D pos, Vector2D vel, float rotation);

	void roundOver();
	void resumeGame();
	void gameOver();
};