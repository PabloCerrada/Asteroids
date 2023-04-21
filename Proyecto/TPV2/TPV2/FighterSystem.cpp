#include "FighterSystem.h"
#include "Manager.h"
#include "BulletSystem.h"
#include "Game.h"

void FighterSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _msg_ROUNDSTART:
		onRoundStart();
		break;
	case _msg_STARTGAME:
		onRoundStart();
		break;
	case _msg_ROUNDOVER:
		onRoundOver();
		break;
	case _msg_COLLISIONFIGHTER:
		onCollision_FighterAsteroid();
		break;
	default:
		break;
	}
}

void FighterSystem::initSystem()
{
	fighter = mngr_->addEntity(_grp_FIGHTER);
	fighter2 = mngr_->addEntity(_grp_FIGHTER);
	mngr_->setHandler(_hdlr_FIGHTER, fighter);
	mngr_->setHandler(_hdlr_FIGHTER2, fighter2);
	fighter->setContext(mngr_);
	fighter2->setContext(mngr_);

	Vector2D velIni = Vector2D(0, 0);
	float width = 44, height = 38.5, rotationIni = 1;
	Vector2D posIni = Vector2D(WIN_WIDTH / 2 - width / 2, WIN_HEIGHT / 2 - height / 2);
	Vector2D posIni2 = Vector2D(WIN_WIDTH - width / 2, WIN_HEIGHT / 2 - height / 2);
	trFighter = mngr_->addComponent<Transform>(fighter, posIni, velIni, width, height, rotationIni);
	trFighter2 = mngr_->addComponent<Transform>(fighter2, posIni2, velIni, width, height, rotationIni);

	int maxLifes = 3;
	mngr_->addComponent<Health>(fighter, maxLifes);
	mngr_->addComponent<Health>(fighter2, maxLifes);
	
	soundThrust = &SDLUtils::instance()->soundEffects().at("thrust");
	soundThrust->setVolume(70);

	soundFire = &SDLUtils::instance()->soundEffects().at("fire");
	soundFire->setVolume(70);

	soundCrash = &SDLUtils::instance()->soundEffects().at("explosion");
	soundCrash->setVolume(70);
	
}

void FighterSystem::updateFighter2(Vector2D pos, Vector2D vel, float rotation)
{
	trFighter2->setPos(pos);
	trFighter2->setVel(vel);
	trFighter2->setR(rotation);
}
void FighterSystem::update()
{
	if (active_)
	{
		trFighter->setPos(trFighter->getPos() + trFighter->getVel());
		SDL_Event event;
		InputHandler::instance()->update(event);
		if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_UP))
		{
			if (sqrt(pow(trFighter->getVel().getX(), 2) + pow(trFighter->getVel().getY(), 2)) < speedLimit)
			{

				trFighter->setVel(trFighter->getVel() + (Vector2D(0, -1).rotate(trFighter->getR()) * acceleration));
			}
			else
			{
				trFighter->setVel(trFighter->getVel().normalize() * speedLimit);
			}
			soundThrust->play();
				
		}

		// Rotation
		if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_LEFT))
		{
			trFighter->setR(trFighter->getR() - 5.0f);
		}
		else if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_RIGHT))
		{
			trFighter->setR(trFighter->getR() + 5.0f);
		}


		//Gun
		if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_S) && canShoot) // Manage time
		{
			canShoot = false;
			lastShootTime = sdlutils().currRealTime();
			Message m;
			m.id = _msg_SHOOT;
			m.bullet.pos = trFighter->getPos() + Vector2D(trFighter->getW() / 2.0f, trFighter->getH() / 2.0f)
				- Vector2D(0.0f, trFighter->getH() / 2.0f + 5.0f + 12.0f).rotate(trFighter->getR())
				- Vector2D(2.0f, 10.0f);
			m.bullet.vel = Vector2D(0.0f, -1.0f).rotate(trFighter->getR()) * (trFighter->getVel().magnitude() + 5.0f);
			m.bullet.rotation = trFighter->getR();
			m.bullet.height = 20;
			m.bullet.width = 5;

				
			soundFire->play();
			mngr_->send(m);
		}
		
		//Gun
		if (lastShootTime + 250 < sdlutils().currRealTime())
		{
			canShoot = true;
		}

		// Deceleration Component
		if (sqrt(pow(trFighter->getVel().getX(), 2) + pow(trFighter->getVel().getY(), 2)) < 0.005)
		{
			trFighter->setVel(Vector2D(0, 0));
		}
		else
		{
			trFighter->setVel(Vector2D(trFighter->getVel().getX() * 0.995, trFighter->getVel().getY() * 0.995));
		}

		//This is the things that were done by ShowAtOpposideSide
		if (trFighter->getPos().getX() > WIN_WIDTH) // Rightside
		{
			trFighter->setPos(Vector2D(-trFighter->getW(), trFighter->getPos().getY()));
		}
		else if (trFighter->getPos().getX() + trFighter->getW() < 0) // Leftside
		{
			trFighter->setPos(Vector2D(WIN_WIDTH, trFighter->getPos().getY()));
		}
		else if (trFighter->getPos().getY() > WIN_HEIGHT) // Downside
		{
			trFighter->setPos(Vector2D(trFighter->getPos().getX(), -trFighter->getH()));
		}
		else if (trFighter->getPos().getY() + trFighter->getH() < 0) // Upside
		{
			trFighter->setPos(Vector2D(trFighter->getPos().getX(), WIN_HEIGHT));
		}
	}	
}

void FighterSystem::onCollision_FighterAsteroid()
{
	trFighter->setPos(Vector2D(WIN_WIDTH / 2 + trFighter->getW() / 2, WIN_HEIGHT / 2 + trFighter->getW() / 2));
	trFighter->setVel(Vector2D(0, 0));
	trFighter->setR(0);
	soundCrash->play();
}

void FighterSystem::onRoundOver()
{
	active_ = false;
}

void FighterSystem::onRoundStart()
{
	active_ = true;
}