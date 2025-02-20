#pragma once
#include "System.h"
#include "Health.h"
class Game;
class GameCtrlSystem : public System
{
private:

	Game* game;
	Entity* fighter;
	Entity* fighter2;

	void deathFighter1();
	void deathFighter2();

	void GameOver();

	// Para gestionar el mensaje de que ha habido un choque entre el fighter y un
	// un asteroide. Tiene que avisar que ha acabado la ronda, quitar una vida
	// al fighter, y si no hay m�s vidas avisar que ha acabado el juego (y quien
	// es el ganador).
	void onCollision_FighterAsteroid();


	// Para gestionar el mensaje de que no hay m�s asteroides. Tiene que avisar que
	// ha acabado la ronda y adem�s que ha acabado el juego (y quien es el ganador)
	void onAsteroidsExtinction();

	int winner_; // 0 - None, 1 - Asteroids, 2- Fighter
	bool winnerMultiplayer;			// true = 1, false = 2
public:
	constexpr static sysId_type id = _sys_GAMECTRL;

	GameCtrlSystem(Game* game_);

	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;

	// Inicializar el sistema, etc.
	void initSystem() override;


	// Si el juego no est� parado y el jugador pulsa SDLK_SPACE cambia el estado
	// como en la pr�ctica 1, etc. Tiene que enviar mensajes correspondientes cuando
	// empieza una ronda o cuando empieza una nueva partida.
	void update() override;
	
};

