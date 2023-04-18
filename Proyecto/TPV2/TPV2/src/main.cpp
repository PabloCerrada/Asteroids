// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>

#include "sdlutils/sdlutils_demo.h"
#include <stdio.h>
#include "../Game.h"


using namespace std;

using uint = unsigned int;


int main(int ac, char **av) {

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // To detect trash

	try {
		Game* game = new Game();
		game->run();
		delete game;
	} catch (const std::string &e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	} catch (const char *e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	} catch (const std::exception &e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}

