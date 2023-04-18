#include "Game.h"

Game::Game() {
	// We first initialize SDL
	SDLUtils::init("Hola", WIN_WIDTH, WIN_HEIGHT, "resources/config/sdlutilsdemo.resources.json");
	renderer = SDLUtils::instance()->renderer();
	window = SDLUtils::instance()->window();


	GameStateMachine::instance()->pushState(new MainMenuState(this));
}
Game::~Game() {
	GameStateMachine::instance()->~GameStateMachine();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();
	while (!exit) {
		frameTime = SDL_GetTicks() - startTime;
		if (frameTime >= FRAME_RATE) {
			update();
			refresh();
			startTime = SDL_GetTicks();
		}
	}
}

void Game::update()
{
	GameStateMachine::instance()->currentState()->update();
}

void Game::refresh()
{
	GameStateMachine::instance()->currentState()->refresh();
}

void Game::setExit() {
	
	exit = !exit;
}

void Game::playFunction(Game* game) {
	
	GameStateMachine::instance()->changeState(new PlayState(game));
}

void Game::exitFunction(Game* game) 
{
	GameStateMachine::instance()->clearState();
	game->setExit();
}

void Game::pauseFunction(Game* game)
{
	GameStateMachine::instance()->pushState(new PauseState(game));
}

void Game::returnToGame(Game* game)
{
	GameStateMachine::instance()->popState();
}

void Game::returnToMainMenu(Game* game)
{
	GameStateMachine::instance()->changeState(new MainMenuState(game));
}