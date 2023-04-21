#include "BulletSystem.h"
#include "Manager.h"
#include "Game.h"

void BulletSystem::initSystem()
{

}

void BulletSystem::receive(const Message& m)
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
	case _msg_SHOOT:
		shoot(m.bullet.pos, m.bullet.vel, m.bullet.width, m.bullet.height, m.bullet.rotation);
		break;
	case _msg_ONCOLLISIONBULLETASTEROID:
		onCollision_BulletAsteroid(m.colision.bullet);
		break;
	default:
		break;
	}
}


void BulletSystem::update()
{
	if (active_)
	{
		for(auto e : mngr_->getEntitiesByGroup(_grp_BULLETS))
		{
			Transform* tr = mngr_->getComponent<Transform>(e);

			tr->setPos(tr->getPos() + tr->getVel());

			// This do the function of DisabledOnExit
			if (tr->getPos().getX() < -tr->getW() || tr->getPos().getX() > WIN_WIDTH || tr->getPos().getY() < -tr->getH() || tr->getPos().getY() > WIN_HEIGHT)
			{
				mngr_->setAlive(e, false);

			}
		}
	}
}

void BulletSystem::shoot(Vector2D pos, Vector2D vel, double width, double height, double rotation)
{
	Entity* bullet = mngr_->addEntity(_grp_BULLETS);
	mngr_->addComponent<Transform>(bullet, pos, vel, width, height, rotation);
}

void BulletSystem::onCollision_BulletAsteroid(Entity* b)
{
	mngr_->setAlive(b, false);
}

void BulletSystem::onRoundOver()
{
	for(auto e : mngr_->getEntitiesByGroup(_grp_BULLETS))
	{
		mngr_->setAlive(e, false);
	}
	active_ = false;
}

void BulletSystem::createBullet(float posX, float posY, float velX, float velY,double rotation,float width,float height)
{
	Entity* bullet = mngr_->addEntity(_grp_BULLETS);
	Vector2D pos = { posX,posY };
	Vector2D vel = { velX,velY };
	mngr_->addComponent<Transform>(bullet, pos, vel, width, height, rotation);
}
void BulletSystem::onRoundStart()
{
	active_ = true;
}