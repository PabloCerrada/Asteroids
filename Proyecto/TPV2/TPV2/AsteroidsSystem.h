#pragma once
#include "System.h"
#include "Entity.h"
#include "ecs.h"
#include "Transform.h"
#include "Generations.h"
#include "Follow.h"
#include "FramedImage.h"

class Manager;
class AsteroidsSystem : public System
{
private:
	int timeGen = 5000;  // Time between the generation of one asteroid
	float lastRespawnTime = 0;
	bool respawn = false;
	bool canCheck = true;
	Transform* trFighter;

	// Para gestionar el mensaje de que ha habido un choque de un asteroide con una
	// bala. Desactivar el asteroide �a� y crear 2 asteroides como en la pr�ctica 1,
	// y si no hay m�s asteroides enviar un mensaje correspondiente.
	void onCollision_AsteroidBullet(Entity* a);

	void onCollision_ExplosionAsteroid(Entity* a);

	// Para gestionar el mensaje de que ha acabado la ronda. Desactivar todos los
	// asteroides, y desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema y
	// a�adir los asteroides iniciales (como en la pr�ctica 1).
	void onRoundStart();

	void createAsteroids(int n);

	// El n�mero actual de asteroides en el juego (recuerda que no puede superar un
	// l�mite)
	int numOfAsteroids_ = 0;


	int killAsteroid = 0;

	// Indica si el sistema est� activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no est� activo)
	bool active_;
public:
	constexpr static sysId_type id = _sys_ASTEROIDS;

	AsteroidsSystem() {};
	virtual ~AsteroidsSystem() {};
	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;
	// Inicializar el sistema, etc.
	void initSystem() override;
	// Si el juego est� parado no hacer nada, en otro caso mover los asteroides como
	// en la pr�ctica 1 y generar 1 asteroide nuevo cada 5 segundos (aparte
	// de los 10 al principio de cada ronda).
	void update() override;
};

