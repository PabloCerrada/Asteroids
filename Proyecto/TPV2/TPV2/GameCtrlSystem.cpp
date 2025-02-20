#include "GameCtrlSystem.h"
#include "Game.h"

GameCtrlSystem::GameCtrlSystem(Game* game_) {
	game = game_;
}

void GameCtrlSystem::initSystem() 
{
	fighter = mngr_->getHandler(_hdlr_FIGHTER);
	if (mngr_->getStateId() == "Multiplayer") {
		fighter2 = mngr_->getHandler(_hdlr_FIGHTER2);
	}
}

void GameCtrlSystem::receive(const Message& m) {
	switch (m.id)
	{
	case _msg_COLLISIONFIGHTER:
		onCollision_FighterAsteroid();
		break;
	case _msg_GAMEOVER:
		GameOver();
		break;
	case _msg_MAINMENU:
		static_cast<PlayState*>(mngr_)->MainMenu();
		break;
	case _msg_DEATHMULTIPLAYER:
		if (m.endOfRound.deathFighter1)
		{	
			deathFighter1();
		}
		else
		{
			deathFighter2();
		}
		break;
	default:
		break;
	}
}

void GameCtrlSystem::update() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && GameStateMachine::instance()->currentState()->getStateId() == "PlayState") 
		{
			game->pauseFunction(game);
		} 
		else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && GameStateMachine::instance()->currentState()->getStateId() == "PauseState")
		{
			game->returnToGame(game);
		}

		else if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE && GameStateMachine::instance()->currentState()->getStateId() != "PlayState")
		{
			game->setExit();
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_n && GameStateMachine::instance()->currentState()->getStateId() == "MainMenuState")
		{
			winner_ = 0;
			game->playFunction(game);
		} 
		else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m && GameStateMachine::instance()->currentState()->getStateId() == "MainMenuState")
		{
			winner_ = 0;
			game->multiplayerFunction(game);
		}

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && GameStateMachine::instance()->currentState()->getStateId() == "PlayState" && winner_ == 1)
		{
			winner_ = 0;
			Message m;
			m.id = _msg_ROUNDSTART;
			mngr_->send(m);
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && GameStateMachine::instance()->currentState()->getStateId() == "Multiplayer" && winner_ == 1)
		{
			winner_ = 0;
			Message m;
			m.id = _msg_ROUNDSTART;
			mngr_->send(m);

			mngr_->getSystem<NetSystem>()->resumeGame();
		}
	}
}

void GameCtrlSystem::deathFighter1() {
	winnerMultiplayer = false;
	winner_ = 1;
	Health* hlth = mngr_->getComponent<Health>(fighter);

	if (hlth->getLifes() >= 2) {
		mngr_->getComponent<Health>(fighter)->quitLife();

		Transform* tr = mngr_->getComponent<Transform>(fighter);
		tr->setPos(Vector2D(tr->getW() + 20, WIN_HEIGHT / 2 + tr->getW() / 2));
		tr->setVel(Vector2D(0, 0));
		tr->setR(0);

		Transform* transformacion = mngr_->getComponent<Transform>(fighter2);
		transformacion->setPos(Vector2D(WIN_WIDTH - tr->getW() - 20, WIN_HEIGHT / 2 + tr->getW() / 2));
		transformacion->setVel(Vector2D(0, 0));
		transformacion->setR(0);

		Message m;
		m.id = _msg_ROUNDOVER;
		mngr_->send(m);

		mngr_->getSystem<NetSystem>()->roundOver();
	}
	else {
		Message m;
		m.id = _msg_GAMEOVERONLINE;
		m.over.fighter1Winner = winnerMultiplayer;
		mngr_->send(m);
	}
}

void GameCtrlSystem::deathFighter2() {
	winnerMultiplayer = true;
	winner_ = 1;
	Health* hlth = mngr_->getComponent<Health>(fighter2);

	if (hlth->getLifes() >= 2) {
		mngr_->getComponent<Health>(fighter2)->quitLife();

		Transform* tr = mngr_->getComponent<Transform>(fighter);
		tr->setPos(Vector2D(tr->getW() + 20, WIN_HEIGHT / 2 + tr->getW() / 2));
		tr->setVel(Vector2D(0, 0));
		tr->setR(0);

		Transform* transformacion = mngr_->getComponent<Transform>(fighter2);
		transformacion->setPos(Vector2D(WIN_WIDTH - tr->getW() - 20, WIN_HEIGHT / 2 + tr->getW() / 2));
		transformacion->setVel(Vector2D(0, 0));
		transformacion->setR(0);

		Message m;
		m.id = _msg_ROUNDOVER;
		mngr_->send(m);

		mngr_->getSystem<NetSystem>()->roundOver();

	}
	else {
		Message m;
		m.id = _msg_GAMEOVERONLINE;
		m.over.fighter1Winner = winnerMultiplayer;
		mngr_->send(m);
	}
}

void GameCtrlSystem::GameOver() {
	static_cast<PlayState*>(mngr_)->setGameover(true);
}

void GameCtrlSystem::onAsteroidsExtinction() {
	winner_ = 2;
}

void GameCtrlSystem::onCollision_FighterAsteroid()
{
	winner_ = 1;
	Health* hlth = mngr_->getComponent<Health>(fighter);
	Transform* tr = mngr_->getComponent<Transform>(fighter);
	
	if (hlth->getLifes() >= 2) {
		mngr_->getComponent<Health>(fighter)->quitLife();

		Message m;
		m.id = _msg_ROUNDOVER;
		mngr_->send(m);
		tr->setPos(Vector2D(WIN_WIDTH / 2 + tr->getW() / 2, WIN_HEIGHT / 2 + tr->getW() / 2));
		tr->setVel(Vector2D(0, 0));
		tr->setR(0);
	}
	else {
		Message m;
		m.id = _msg_GAMEOVER;
		mngr_->send(m);
	}
}