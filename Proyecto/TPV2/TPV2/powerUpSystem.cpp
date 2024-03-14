#include "powerUpSystem.h"
#include "Manager.h"
#include "Game.h"

void powerUpSystem::initSystem()
{

}
void powerUpSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _msg_ROUNDOVER:
		onRoundOver();
		break;
	case _msg_STARTGAME:
		onRoundStart();
		break;
	case _msg_ROUNDSTART:
		onRoundStart();
		break;
	case _msg_CREATEPOWERUP:
		createPowerUp(1);
		break;
	case _msg_COLLISIONPOWERUP:
		onCollision_FighterPower(m.powerUp.powerUp);
	}
}

void powerUpSystem::update()
{
	if (active_)
	{
		
		for (auto e : mngr_->getEntitiesByGroup(_grp_POWERUPS))
		{
			powerUpComponent* power= mngr_->getComponent<powerUpComponent>(e);
			power->setTimer(power->getTimer() + 1);
			
			if (power->getTimer()> 1000)
			{
				mngr_->setAlive(e, false);
			}
		}

	}
}
void powerUpSystem::onCollision_FighterPower(Entity* a)
{
	mngr_->setAlive(a, false);
}

void powerUpSystem::onRoundOver()
{
	active_ = false;
}

void powerUpSystem::onRoundStart()
{
	active_ = true;
}

void powerUpSystem::createPowerUp(int n)
{
	
	int numero = rand() % 2 +1;
	
	Entity* a = mngr_->addEntity(_grp_POWERUPS);
	float posX = (WIN_WIDTH / 2) + int(100 * numPowerUps);
	Vector2D pos{posX ,WIN_HEIGHT / 2};
	numPowerUps++;
	Vector2D vel{ 0,0 };
	
	mngr_->addComponent<Transform>(a,pos,vel,90,60,0);			
	mngr_->addComponent<powerUpComponent>(a,numero);
}