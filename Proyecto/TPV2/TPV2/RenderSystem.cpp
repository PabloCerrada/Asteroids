#include "RenderSystem.h"
#include "Game.h"

RenderSystem::RenderSystem() {
	winner_ = 0;
}

void RenderSystem::initSystem() {
	font = &SDLUtils::instance()->fonts().at("ARIAL16");
	healthTexture = &SDLUtils::instance()->images().at("heart");
	if (mngr_->getStateId() == "PlayState") {
		fighterTransform = mngr_->getComponent<Transform>(mngr_->getHandler(_hdlr_FIGHTER));
		lives = mngr_->getComponent<Health>(mngr_->getHandler(_hdlr_FIGHTER))->getLifes();
		fighterTexture = &SDLUtils::instance()->images().at("fighter");
		bulletTexture = &SDLUtils::instance()->images().at("fire");
		asteroidTexture = &SDLUtils::instance()->images().at("asteroid");
		asteroidGoldTexture = &SDLUtils::instance()->images().at("asteroidGold");
	}
}

void RenderSystem::receive(const Message& m) {
	switch (m.id)
	{
	case _msg_ROUNDOVER:
		onRoundOver();
		break;
	case _msg_ROUNDSTART:
		onRoundStart();
		break;
	case _msg_GAMEOVER:
		onGameOver();
		break;
	case _msg_WIN:
		onGameWin();
		break;
	default:
		break;
	}
}

void RenderSystem::update() {

	SDL_RenderClear(SDLUtils::instance()->renderer());
	if (GameStateMachine::instance()->currentState()->getStateId() == "PlayState")
	{
		for (int i = 0; i < lives; i++) // Render lives
		{
			Vector2D pos = Vector2D(55 * i + 5, 10);
			SDL_Rect destRect = build_sdlrect(pos, 50, 50);
			healthTexture->render(destRect);
		}
		if (winner_ == 0) {
			SDL_Rect dest = build_sdlrect(fighterTransform->getPos(), fighterTransform->getW(), fighterTransform->getH());
			fighterTexture->render(dest, fighterTransform->getR());

			// Render bullets
			for (auto it : mngr_->getEntitiesByGroup(_grp_BULLETS)) { 
				Transform* bulletTransform = mngr_->getComponent<Transform>(it);
				SDL_Rect dest = build_sdlrect(bulletTransform->getPos(), bulletTransform->getW(), bulletTransform->getH());
				bulletTexture->render(dest, bulletTransform->getR());
			}
			// Render asteroids
			for (auto it : mngr_->getEntitiesByGroup(_grp_ASTEROIDS)) {
				Transform* asteroidTransform = mngr_->getComponent<Transform>(it);
				FramedImage* asteroidImage = mngr_->getComponent<FramedImage>(it);
				SDL_Rect dest, src;

				dest.x = asteroidTransform->getPos().getX();
				dest.y = asteroidTransform->getPos().getY();
				dest.h = asteroidTransform->getH();
				dest.w = asteroidTransform->getW();

				src.x = asteroidImage->getCol() * (asteroidImage->getWidthFrame());
				src.y = asteroidImage->getRow() * (asteroidImage->getHeightFrame());
				src.h = asteroidImage->getHeightFrame();
				src.w = asteroidImage->getWidthFrame();

				//This if will be used to change the row to render
				if (asteroidImage->getCol() == asteroidImage->getNFrames() && asteroidImage->getCont() >= 2)
				{
					asteroidImage->setCol(0);
					asteroidImage->setRow((asteroidImage->getRow() + 1) % asteroidImage->getNFrames());
				}

				//We change the col
				if (asteroidImage->getCont() >= asteroidImage->getNFrames())
				{
					asteroidImage->setCol(asteroidImage->getCol() + 1);
					asteroidImage->setCont(0);
				}
				asteroidImage->setCont(asteroidImage->getCont() + 1);

				if (mngr_->hasComponent<Follow>(it)) {
					asteroidGoldTexture->render(src, dest, 0, nullptr);
				}
				else {
					asteroidTexture->render(src, dest, 0, nullptr);
				}
			}
		}
		// Render when asteoird crash into fighter and you have more lives
		else if (winner_ == 1) {
			font->render(SDLUtils::instance()->renderer(), "PRESS SPACE TO CONTINUE", (WIN_WIDTH / 2) - 100, WIN_HEIGHT / 2, s);
		}
	}
	// Render Pause
	else if (GameStateMachine::instance()->currentState()->getStateId() == "PauseState") {
		font->render(SDLUtils::instance()->renderer(), "PAUSE: PRESS SPACE TO CONTINUE", (WIN_WIDTH / 2) - 100, WIN_HEIGHT / 2, s);
	}
	// Render MainMenu
	else if (GameStateMachine::instance()->currentState()->getStateId() == "MainMenuState") {
		font->render(SDLUtils::instance()->renderer(), "MENU: PRESS SPACE TO SINGLEPLAYER", (WIN_WIDTH / 2) - 150, WIN_HEIGHT / 2 - 50, s);
		font->render(SDLUtils::instance()->renderer(), "MENU: PRESS M TO MULTIPLAYER", (WIN_WIDTH / 2) - 150, WIN_HEIGHT / 2 + 50, s);
	}
	SDL_RenderPresent(SDLUtils::instance()->renderer());
}

void RenderSystem::onGameOver()
{
	SDL_RenderClear(SDLUtils::instance()->renderer());
	font->render(SDLUtils::instance()->renderer(), "YOU LOST!", (WIN_WIDTH / 2) - 50, WIN_HEIGHT / 2, s);
	SDL_RenderPresent(SDLUtils::instance()->renderer());
	SDL_Delay(5000);
	Message m;
	m.id = _msg_MAINMENU;
	mngr_->send(m);
}

void RenderSystem::onRoundOver() 
{
	winner_ = 1;
}

void RenderSystem::onGameWin() {
	winner_ = 2;
	SDL_RenderClear(SDLUtils::instance()->renderer());
	font->render(SDLUtils::instance()->renderer(), "YOU WIN!", (WIN_WIDTH / 2) - 50, WIN_HEIGHT / 2, s);
	SDL_RenderPresent(SDLUtils::instance()->renderer());
	SDL_Delay(5000);
	Message m;
	m.id = _msg_MAINMENU;
	mngr_->send(m);
}

void RenderSystem::onRoundStart()
{
	lives = mngr_->getComponent<Health>(mngr_->getHandler(_hdlr_FIGHTER))->getLifes();
	winner_ = 0;
}