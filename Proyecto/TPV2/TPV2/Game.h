#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "src/utils/checkML.h"
#include "src/sdlutils/Texture.h"
#include "src/utils/Vector2D.h"
#include <math.h>
#include <list>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "GameStateMachine.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "MultiplayerState.h"
#include <SDL_net.h>


using namespace std;
using uint = unsigned int;

const uint WIN_WIDTH = 800;
const uint WIN_HEIGHT = 600;
const uint FRAME_RATE = 10;
const uint ASTEROID_WIDTH_FRAME = 85;
const uint ASTEROID_HEIGHT_FRAME = 100;
const uint ASTEROID_NFRAMES = 5;

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool end = false; // control booleans
public:
	Game();
	~Game();
	void run();
	void update();
	void refresh();
	void setExit();
	static void multiplayerFunction(Game* game);
	static void playFunction(Game* game);
	static void exitFunction(Game* game);
	static void pauseFunction(Game* game);
	static void returnToGame(Game* game);
	static void returnToMainMenu(Game* game);

	void server(int port);
	void client(char* host, int port);

};

