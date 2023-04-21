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
			{
				ShootMessage* m = static_cast<ShootMessage*>(message);
				m = reinterpret_cast<ShootMessage*>(p_->data);
				mngr_->getSystem<BulletSystem>()->createBullet(m->x, m->y, m->velX, m->velY, m->rot, m->width, m->height);
				break;
			}
			case _net_NEWFIGHERPOS_:
			{
				cout << "as";
				FighterPosMessage* me = static_cast<FighterPosMessage*>(message);
				me = reinterpret_cast<FighterPosMessage*>(p_->data);
				Vector2D pos = { me->x,me->y };
				Vector2D vel = { me->velX,me->velY };
				mngr_->getSystem<FighterSystem>()->updateFighter2(pos, vel, me->rot);
				break;
			}		
			default:
				break;
		}
	}
}

void NetSystem::setFighter(Vector2D pos, Vector2D vel, float rotation)
{
	FighterPosMessage* me = static_cast<FighterPosMessage*>(message);
	me->id = _net_NEWFIGHERPOS_;
	me->x = pos.getX();
	me->y = pos.getY();

	me->velX = vel.getX();
	me->velY = vel.getY();

	me->rot = rotation;

	me->width = 44;
	me->height = 38.5;
	
	p_->len = sizeof(FighterPosMessage);
	SDLNet_UDP_Send(sock_, -1, p_);
}

void NetSystem::createBullet(Vector2D pos, Vector2D vel, float rotation)
{
	ShootMessage* me = static_cast<ShootMessage*>(message);
	me->id = _net_BULLETSHOT;

	me->x = pos.getX();
	me->y = pos.getY();

	me->velX = vel.getX();
	me->velY = vel.getY();

	me->rot = rotation;

	me->height = 20;
	me->width = 5;

	p_->len = sizeof(ShootMessage);
	SDLNet_UDP_Send(sock_, -1, p_);




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
	sock_ = SDLNet_UDP_Open(0);			//busca el puerto que este abierto
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



	
	p_->address = ip;
	SDLNet_UDP_Send(sock_, -1, p_);
	
}