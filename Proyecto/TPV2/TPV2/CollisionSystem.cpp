#include "CollisionSystem.h"
#include "Manager.h"

void CollisionSystem::initSystem()
{
	trFighter = mngr_->getComponent<Transform>(mngr_->getHandler(_hdlr_FIGHTER));
	if (mngr_->getStateId()=="Multiplayer")
	{
		trFighter2 = mngr_->getComponent<Transform>(mngr_->getHandler(_hdlr_FIGHTER2));
	}

	soundCrash = &SDLUtils::instance()->soundEffects().at("explosion");
	soundCrash->setVolume(30);
}


void CollisionSystem::receive(const Message& m)
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
	}
}

void CollisionSystem::update()
{
	if (active_)
	{
		checkCollision();
	}
}

void CollisionSystem::onRoundOver()
{
	active_ = false;
}

void CollisionSystem::onRoundStart()
{
	active_ = true;
}

// Method that manage collision asteroid - bullet and asteroid - player - bullet and player
void CollisionSystem::checkCollision() { 
	if (mngr_->getStateId()=="PlayState")
	{

		for (auto ast : mngr_->getEntitiesByGroup(_grp_ASTEROIDS)) {
			Transform* astTr = mngr_->getComponent<Transform>(ast);
			for (auto bull : mngr_->getEntitiesByGroup(_grp_BULLETS)) {

				Transform* bullTr = mngr_->getComponent<Transform>(bull);
				if (Collisions::collidesWithRotation(astTr->getPos(), astTr->getW(), astTr->getH(), astTr->getR(), bullTr->getPos(), bullTr->getW(), bullTr->getH(), bullTr->getR())) {
					Message m;
					m.id = _msg_ONCOLLISIONBULLETASTEROID;
					m.colision.asteroid = ast;
					m.colision.bullet = bull;
					mngr_->send(m);
				}
			}
			if (Collisions::collidesWithRotation(astTr->getPos(), astTr->getW(), astTr->getH(), astTr->getR(), trFighter->getPos(), trFighter->getW(), trFighter->getH(), trFighter->getR())) {
				soundCrash->play();
				Message m;
				m.id = _msg_COLLISIONFIGHTER;
				mngr_->send(m);
			}
		}
	}
	else if (mngr_->getStateId() == "Multiplayer")
	{
		for (auto bull : mngr_->getEntitiesByGroup(_grp_BULLETS))
		{
			Transform* bullTr = mngr_->getComponent<Transform>(bull);
			if (Collisions::collidesWithRotation(trFighter->getPos(), trFighter->getW(), trFighter->getH(), trFighter->getR(), bullTr->getPos(), bullTr->getW(), bullTr->getH(), bullTr->getR())) {
				Message m;
				m.id = _msg_DEATHMULTIPLAYER;
				m.endOfRound.deathFighter1 = true;
				m.endOfRound.deathFighter2 = false;
				mngr_->send(m);
			}
			else if(Collisions::collidesWithRotation(trFighter2->getPos(), trFighter2->getW(), trFighter2->getH(), trFighter2->getR(), bullTr->getPos(), bullTr->getW(), bullTr->getH(), bullTr->getR())) {
				Message m;
				m.id = _msg_DEATHMULTIPLAYER;
				m.endOfRound.deathFighter1 = false;
				m.endOfRound.deathFighter2 = true;
				mngr_->send(m);
			}
		}
	}
}



