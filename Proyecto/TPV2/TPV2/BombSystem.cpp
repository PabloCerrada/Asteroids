#include "BombSystem.h"
#include "Manager.h"
#include "Game.h"



void BombSystem::initSystem()
{

}
void BombSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _msg_STARTGAME:
		onRoundStart();
		break;
	case _msg_ROUNDOVER:
		onRoundOver();
		break;
	case _msg_ROUNDSTART:
		onRoundStart();
		break;
	case _msg_BOMB:
		shoot(m.bomb.pos, m.bomb.vel, m.bomb.width, m.bomb.height, m.bomb.rotation);
		break;
	case _msg_COLLISIONBOMBASTEROID:
		onCollision_BombAsteroid(m.colision.bullet);
		break;
	default:
		break;
	}
}


void BombSystem::onRoundOver()
{
	active_ = false;
}

void BombSystem::onRoundStart()
{
	active_ = true;
}

void BombSystem::update()
{
	if (active_)
	{
		for (auto e : mngr_->getEntitiesByGroup(_grp_BOMB))
		{
			Transform* tr = mngr_->getComponent<Transform>(e);

			tr->setPos(tr->getPos() + tr->getVel());

			// This do the function of DisabledOnExit
			if (tr->getPos().getX() < -tr->getW() || tr->getPos().getX() > WIN_WIDTH || tr->getPos().getY() < -tr->getH() || tr->getPos().getY() > WIN_HEIGHT)
			{
				mngr_->setAlive(e, false);

			}
		}
		for (auto e : mngr_->getEntitiesByGroup(_grp_EXPLOSION))
		{
			ExplosionTimer* explosionTimer = mngr_->getComponent<ExplosionTimer>(e);
			explosionTimer->setDeathTimer(explosionTimer->getDeathTimer() + 1);
			
			if (explosionTimer->getDeathTimer()>100)
			{
				mngr_->setAlive(e, false);
			}
		}

	}
}
void BombSystem::shoot(Vector2D pos, Vector2D vel, double width, double height, double rotation)
{
	Entity* bomb = mngr_->addEntity(_grp_BOMB);
	mngr_->addComponent<Transform>(bomb, pos, vel, width, height, rotation);
}

void BombSystem::onCollision_BombAsteroid(Entity* ent)
{
	//set alive a false de la bomba y creación de la explosión
	Transform* trExplosion = mngr_->getComponent<Transform>(ent);
	Vector2D pos = trExplosion->getPos();
	pos.setX(pos.getX() - trExplosion->getW() / 2);
	Vector2D vel = trExplosion->getVel();
	mngr_->setAlive(ent, false);

	Entity* explosion = mngr_->addEntity(_grp_EXPLOSION);
	mngr_->addComponent<ExplosionTimer>(explosion);
	mngr_->addComponent<Transform>(explosion, pos, vel, 93, 60, 0);

	
}