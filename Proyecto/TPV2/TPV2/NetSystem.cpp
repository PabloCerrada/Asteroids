#include "NetSystem.h"
#include "Game.h"
#include "BulletSystem.h"
#include "FighterSystem.h"
NetSystem::NetSystem()
{
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
		imServer = true;
		server(port_);
	}
	else {
		imServer = false;
		// Pregunta por la IP
		cout << "IP: ";
		char host[1024];
		cin >> host;
		client(host, port_);
	}
}

void NetSystem:: update()
{
	
	while (SDLNet_UDP_Recv(sock_,p_)>0)
	{
		switch(message->id)
		{

		case _net_BULLETSHOT:
			m = reinterpret_cast<ShootMessage*>(p_->data);
			mngr_->getSystem<BulletSystem>()->createBullet(m->x, m->y, m->velX, m->velY, m->rot, m->width, m->height);
			break;
		case _net_NEWFIGHERPOS_:
			me = reinterpret_cast<FighterPosMessage*>(p_->data);
			/*mngr_->getSystem<FighterSystem>()*/
			break;
		default:
			break;
		}
	}
}

void NetSystem::server(int port)
{
	sock_ = SDLNet_UDP_Open(port);
	if(!sock_)
	{
		throw "asdasd";
	}
	p_ = SDLNet_AllocPacket(MAXPACKETSIZE);
	if (!p_)
	{
		throw "asdasd";
	}
	message = reinterpret_cast<NetMessage*>(p_->data);
	sockSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(sockSet_, sock_);
}
void NetSystem::client(char* host, int port)
{
	sock_ = SDLNet_UDP_Open(0);
	if (!sock_)
	{
		throw "asdasd";
	}
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, host, port) < 0) {			//ip del cliente
		throw "ERROR AL ESTABLECER LA CONEXION CON EL SERVIDOR SIENDO CLIENTE";
	}
	p_ = SDLNet_AllocPacket(MAXPACKETSIZE);
	if (!p_)
	{
		throw "asdasd";
	}
	message = reinterpret_cast<NetMessage*>(p_->data);
	sockSet_ = SDLNet_AllocSocketSet(1);
	SDLNet_UDP_AddSocket(sockSet_, sock_);
	//ENVIAS MENSAJE DICIENDO QUE QUIERES JUGAR con UDPSEND	
}