#pragma once
#include "src/utils/Vector2D.h"
#ifndef ECS_H_
#define ECS_H_

class Entity;			//Si incluyo entity rompe todo
using msgId_type = int;
enum msgId : msgId_type {
	_msg_STARTGAME = 0,
	_msg_GAMEOVER = 1,
	_msg_ROUNDSTART = 2,
	_msg_ROUNDOVER = 3,
	_msg_WIN = 4,
	_msg_ONCOLLISIONBULLETASTEROID = 5,
	_msg_COLLISIONFIGHTER = 6,
	_msg_SHOOT = 7,	
	_msg_MAINMENU = 8,
	_msg_DEATHMULTIPLAYER =9,
	_msg_GAMEOVERONLINE = 10,
	_msg_BOMB = 11,
	_msg_COLLISIONBOMBASTEROID=12,
	_msg_COLLISIONEXPLOSIONASTEROID=13,
	_msg_CREATEPOWERUP =14,
	_msg_COLLISIONPOWERUP = 15,
	_LAST_MSG_ID
};

using netMsgId_type = int;
enum netMessage : netMsgId_type
{
	_net_BULLETSHOT = 0,
	_net_NEWFIGHERPOS_ = 1,
	_net_ROUNDOVER = 2,
	_net_RESUMEGAME = 3,
	_net_CONNECT = 4,
	_net_GAMEOVER = 5,

	_LAST_NETMSG_ID
};
constexpr netMsgId_type maxNetMsg= _LAST_NETMSG_ID;

using cmpId_type = int;
enum cmpId : cmpId_type {
	// ... (compoment ids)
	TRANSFORM_H = 0,
	DEACCELERATIONCOMPONENT_H = 1,
	IMAGE_H = 2,
	HEALTH_H = 3,
	FIGHTERCONTROL_H = 4,
	SHOWATOPPOSIDESIDE_H = 5,
	GUN_H = 6,
	DISABLEONEXIT_H = 7,
	FRAMEDIMAGE_H = 8,
	FOLLOW_H = 9,
	GENERATIONS_H = 10,
	EXPLOSIONTIMER_H = 11, 
	POWERUPCOMPONENT_H =12,
	// do not remove this
	_LAST_CMP_ID
};
constexpr cmpId_type maxComponentId = _LAST_CMP_ID;

using grpId_type = int;
enum grpId : grpId_type {
	_grp_ASTEROIDS = 0,
	_grp_BULLETS = 1,
	_grp_FIGHTER = 2,
	_grp_BOMB =3,
	_grp_EXPLOSION=4,
	_grp_POWERUPS =5,
	_LAST_GRP_ID
};
constexpr grpId_type maxGroupId = _LAST_GRP_ID;

using sysId_type = int;
enum sysId : sysId_type {
	_sys_ASTEROIDS = 0,
	_sys_COLLISIONS = 1,
	_sys_RENDER = 2,
	_sys_GAMECTRL = 3,
	_sys_FIGHTER = 4,
	_sys_BULLET = 5,
	_sys_NET = 6,
	_sys_BOMB = 7, 
	_sys_POWERUP=8,
	// do not remove this
	_LAST_SYS_ID
};
constexpr sysId_type maxSystemId = _LAST_SYS_ID;

using hdlrId_type = int;
enum hdlrId : hdlrId_type {

	_hdlr_FIGHTER = 0,
	_hdlr_FIGHTER2=1,
	// do not remove this
	_LAST_HDLR_ID
};
constexpr hdlrId_type maxHandlerId = _LAST_HDLR_ID;
#endif // !ECS_H_

