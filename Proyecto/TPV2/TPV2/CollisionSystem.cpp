#include "CollisionSystem.h"
#include "Manager.h"

void CollisionSystem::initSystem()
{
	trFighter = mngr_->getComponent<Transform>(mngr_->getHandler(_hdlr_FIGHTER));
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

// Method that manage collision asteroid - bullet and asteroid - player
void CollisionSystem::checkCollision() { 
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
			Message m;
			m.id =_msg_COLLISIONFIGHTER;
			mngr_->send(m);
		}
	}
}



