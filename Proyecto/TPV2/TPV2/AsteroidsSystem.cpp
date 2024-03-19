#include "AsteroidsSystem.h"
#include "Manager.h"
#include "Game.h"

void AsteroidsSystem::initSystem()
{
	trFighter = mngr_->getComponent<Transform>(mngr_->getHandler(_hdlr_FIGHTER));
}

void AsteroidsSystem::receive(const Message& m)
{
	switch (m.id)
	{
	case _msg_STARTGAME: 
		onRoundStart();
		break;
	case _msg_ROUNDSTART:
		onRoundStart();
		break;
	case _msg_ROUNDOVER:
		onRoundOver();
		break;
	case _msg_COLLISIONEXPLOSIONASTEROID:
		onCollision_ExplosionAsteroid(m.explosion.asteroid);
	case _msg_ONCOLLISIONBULLETASTEROID:
		onCollision_AsteroidBullet(m.colision.asteroid);
		break;
	case _msg_COLLISIONBOMBASTEROID:
		onCollision_ExplosionAsteroid(m.colision.asteroid);
	default:
		break;
	}
}


void AsteroidsSystem::update()
{
	if (active_) {
		for (auto e : mngr_->getEntitiesByGroup(_grp_ASTEROIDS))
		{
			auto tr = mngr_->getComponent<Transform>(e);

			// This do the function od ShowAtOppositeSide
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

			// ASteroids that follows the fighter
			if (mngr_->hasComponent<Follow>(e)) {
				tr->setVel(tr->getVel().rotate(tr->getVel().angle(trFighter->getPos() - tr->getPos()) > 0 ? 1.0f : -1.0f));
			}

			tr->setPos(tr->getVel() + tr->getPos());
		}
		if (lastRespawnTime + timeGen < sdlutils().currRealTime()) respawn = true;
		if (respawn) {
			respawn = false;
			lastRespawnTime = sdlutils().currRealTime();
			createAsteroids(1);
		}
		if (numOfAsteroids_ <= 0) {
			Message m;
			m.id = _msg_WIN;
			mngr_->send(m);
		}

		if (killAsteroid>5)
		{
			killAsteroid = 0;
			Message m;
			m.id = _msg_CREATEPOWERUP;
			mngr_->send(m);

		}
		canCheck = true;
	}
}
void AsteroidsSystem::onRoundStart()
{
	active_ = true;
    lastRespawnTime = sdlutils().currRealTime();
	createAsteroids(10);
}


void AsteroidsSystem::onCollision_ExplosionAsteroid(Entity* a)
{
	killAsteroid++;
	numOfAsteroids_--;
	mngr_->setAlive(a, false);
	canCheck = false;
}

void AsteroidsSystem::onCollision_AsteroidBullet(Entity* ent)
{
	if (canCheck)
	{
		int gen = mngr_->getComponent<Generations>(ent)->getGenerations();
		if (gen > 1 && mngr_->getEntities().size() < 29) {
			for (int i = 0; i < 2; i++) {
				Transform* tr = mngr_->getComponent<Transform>(ent);
				++numOfAsteroids_;
				Entity* asteroid = mngr_->addEntity(_grp_ASTEROIDS);
				int newGen = gen - 1;

				mngr_->addComponent<Generations>(asteroid, newGen);

				int newWidth = (tr->getW() / gen) * newGen, newHeight = (tr->getH() / gen) * newGen;

				auto r = sdlutils().rand().nextInt(0, 360);
				auto pos = tr->getPos();
				auto vel = tr->getVel().rotate(r) * 1.1f;

				mngr_->addComponent<Transform>(asteroid, pos, vel, newWidth, newHeight, r);

				if (sdlutils().rand().nextInt(0, 10) < 3)
				{
					mngr_->addComponent<Follow>(asteroid);
				}

				mngr_->addComponent<FramedImage>(asteroid, ASTEROID_WIDTH_FRAME, ASTEROID_HEIGHT_FRAME, ASTEROID_NFRAMES);
			}
		}
		--numOfAsteroids_;
		killAsteroid++;
		mngr_->setAlive(ent, false);

	}
}
	

void AsteroidsSystem::onRoundOver()
{
	for (auto& e : mngr_->getEntitiesByGroup(_grp_ASTEROIDS)) 
	{
		mngr_->setAlive(e, false);
	}
	numOfAsteroids_ = 0;
	active_ = false;	
}


void AsteroidsSystem::createAsteroids(int n)
{
	numOfAsteroids_ += n;
	for (int i = 0; i < n; i++)
	{
		Entity* asteroid = mngr_->addEntity(_grp_ASTEROIDS);

		int gen = sdlutils().rand().nextInt(1, 4); // Number of generations of the asteoird

		mngr_->addComponent<Generations>(asteroid, gen);

		// Code to decide where the asteoird spawn
		int positionProb = sdlutils().rand().nextInt(0, 4);
		Vector2D posIni;
		switch (positionProb) {
		case 0: // Upside
			posIni = Vector2D(sdlutils().rand().nextInt(0, WIN_WIDTH), sdlutils().rand().nextInt(0, 100));
			break;
		case 1: // Rightside
			posIni = Vector2D(sdlutils().rand().nextInt(WIN_WIDTH - 100, WIN_WIDTH), sdlutils().rand().nextInt(0, WIN_HEIGHT));
			break;
		case 2: // Leftside
			posIni = Vector2D(sdlutils().rand().nextInt(0, 100), sdlutils().rand().nextInt(0, WIN_HEIGHT));
			break;
		case 3: // Downside
			posIni = Vector2D(sdlutils().rand().nextInt(0, WIN_WIDTH), sdlutils().rand().nextInt(WIN_HEIGHT - 100, WIN_HEIGHT));
			break;
		}

		Vector2D h = Vector2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
		Vector2D r = Vector2D(sdlutils().rand().nextInt(-100, 101), sdlutils().rand().nextInt(-100, 101));
		Vector2D c = h + r;
		float speed = sdlutils().rand().nextInt(5, 10) / 10.0f;
		Vector2D velIni = (c - posIni).normalize() * speed;
		float width = 17 * gen, height = 20 * gen, rotationIni = 1;

		mngr_->addComponent<Transform>(asteroid, posIni, velIni, width, height, rotationIni);

		if (sdlutils().rand().nextInt(0, 10) < 3)
		{
			mngr_->addComponent<Follow>(asteroid);
		}

		mngr_->addComponent<FramedImage>(asteroid, ASTEROID_WIDTH_FRAME, ASTEROID_HEIGHT_FRAME, ASTEROID_NFRAMES);
	}
}








