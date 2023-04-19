#include "GameCtrlSystem.h"
#include "Game.h"

GameCtrlSystem::GameCtrlSystem(Game* game_) {
	game = game_;
}

void GameCtrlSystem::initSystem() 
{
	fighter = mngr_->getHandler(_hdlr_FIGHTER);
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

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && GameStateMachine::instance()->currentState()->getStateId() == "MainMenuState")
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
	
	if (hlth->getLifes() >= 1) {
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