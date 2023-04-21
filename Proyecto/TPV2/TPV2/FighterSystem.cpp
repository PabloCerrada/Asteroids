#include "FighterSystem.h"
#include "Manager.h"
#include "BulletSystem.h"
#include "Game.h"
#include "NetSystem.h"
#include "NetMessage.h"

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
	mngr_->setHandler(_hdlr_FIGHTER, fighter);
	fighter->setContext(mngr_);
	Vector2D velIni = Vector2D(0, 0);
	float width = 44, height = 38.5, rotationIni = 1;
	Vector2D posIni = Vector2D(WIN_WIDTH / 2 - width / 2, WIN_HEIGHT / 2 - height / 2);
	trFighter = mngr_->addComponent<Transform>(fighter, posIni, velIni, width, height, rotationIni);
	int maxLifes = 3;
	mngr_->addComponent<Health>(fighter, maxLifes);
	if (mngr_->getStateId()=="Multiplayer")
	{
		fighter2 = mngr_->addEntity(_grp_FIGHTER);
		mngr_->setHandler(_hdlr_FIGHTER2, fighter2);
		fighter2->setContext(mngr_);
		Vector2D posIni2 = Vector2D(WIN_WIDTH - width / 2, WIN_HEIGHT / 2 - height / 2);
		trFighter2 = mngr_->addComponent<Transform>(fighter2, posIni2, velIni, width, height, rotationIni);
		mngr_->addComponent<Health>(fighter2, maxLifes);
	}
	
	
	soundThrust = &SDLUtils::instance()->soundEffects().at("thrust");
	soundThrust->setVolume(0);

	soundFire = &SDLUtils::instance()->soundEffects().at("fire");
	soundFire->setVolume(0);

	soundCrash = &SDLUtils::instance()->soundEffects().at("explosion");
	soundCrash->setVolume(0);
	
}
void FighterSystem:: fighterActions(Entity* ent_)
{
	Transform* tr = mngr_->getComponent<Transform>(ent_);

	tr->setPos(tr->getPos() + tr->getVel());
	
	SDL_Event event;
	InputHandler::instance()->update(event);
	if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_UP))
	{
		if (sqrt(pow(tr->getVel().getX(), 2) + pow(tr->getVel().getY(), 2)) < speedLimit)
		{
			tr->setVel(tr->getVel() + (Vector2D(0, -1).rotate(tr->getR()) * acceleration));
		}
		else
		{
			tr->setVel(tr->getVel().normalize() * speedLimit);
		}
		soundThrust->play();

	}

	// Rotation
	if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_LEFT))
	{
		tr->setR(tr->getR() - 5.0f);
	}
	else if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_RIGHT))
	{
		tr->setR(tr->getR() + 5.0f);
	}


	//Gun
	if (InputHandler::instance()->isKeyDown(SDL_SCANCODE_S) && canShoot) // Manage time
	{
		canShoot = false;
		lastShootTime = sdlutils().currRealTime();
		Message m;
		m.id = _msg_SHOOT;
		m.bullet.pos = tr->getPos() + Vector2D(tr->getW() / 2.0f, tr->getH() / 2.0f)
			- Vector2D(0.0f, tr->getH() / 2.0f + 5.0f + 12.0f).rotate(tr->getR())
			- Vector2D(2.0f, 10.0f);
		m.bullet.vel = Vector2D(0.0f, -1.0f).rotate(tr->getR()) * (tr->getVel().magnitude() + 5.0f);
		m.bullet.rotation = tr->getR();
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
	if (sqrt(pow(tr->getVel().getX(), 2) + pow(tr->getVel().getY(), 2)) < 0.005)
	{
		tr->setVel(Vector2D(0, 0));
	}
	else
	{
		tr->setVel(Vector2D(tr->getVel().getX() * 0.995, tr->getVel().getY() * 0.995));
	}

	//This is the things that were done by ShowAtOpposideSide
	if (tr->getPos().getX() > WIN_WIDTH) // Rightside
	{
		tr->setPos(Vector2D(-tr->getW(), tr->getPos().getY()));
	}
	else if (tr->getPos().getX() + tr->getW() < 0) // Leftside
	{
		tr->setPos(Vector2D(WIN_WIDTH, tr->getPos().getY()));
	}
	else if (tr->getPos().getY() > WIN_HEIGHT) // Downside
	{
		tr->setPos(Vector2D(tr->getPos().getX(), -tr->getH()));
	}
	else if (tr->getPos().getY() + tr->getH() < 0) // Upside
	{
		tr->setPos(Vector2D(tr->getPos().getX(), WIN_HEIGHT));
	}

	if(GameStateMachine::instance()->currentState()->getStateId() == "Multiplayer")
	{
		mngr_->getSystem<NetSystem>()->setFighter(tr->getPos(), tr->getVel(), tr->getR());
	}
	
}

void FighterSystem::updateFighter2(Vector2D pos, Vector2D vel, float rotation)
{
	if (!mngr_->getSystem<NetSystem>()->isServer())
	{
		trFighter->setPos(pos);
		trFighter->setVel(vel);
		trFighter->setR(rotation);
	}
	else
	{
		trFighter2->setPos(pos);
		trFighter2->setVel(vel);
		trFighter2->setR(rotation);
	}
}
void FighterSystem::update()
{
	if (active_)
	{
		if (GameStateMachine::instance()->currentState()->getStateId() == "Multiplayer")
		{
			if (mngr_->getSystem<NetSystem>()->isServer())
			{
				fighterActions(fighter);
			}
			else
			{
				fighterActions(fighter2);
			}	
		}
		else
		{
			fighterActions(fighter);
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