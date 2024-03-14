#pragma once
#include "System.h"
#include "ecs.h"
#include "Transform.h"
#include "src/sdlutils/InputHandler.h"
#include "Health.h"

class BulletSystem;
class Manager;

class FighterSystem :public System
{
private:
	SoundEffect* soundThrust;
	SoundEffect* soundFire;
	
	Entity* fighter;
	Entity* fighter2;

	float powerUpDuratiion;
	bool powerUp = false;
	float lastShootTime;
	bool canShoot = false;
	float cadence = 1;
	int actualPowerUp = 0;
	Transform* trFighter;
	Transform* trFighter2;
	float acceleration = 0.2f, speedLimit = 3.0f;
	
	// Para reaccionar al mensaje de que ha habido un choque entre el fighter y un
	// un asteroide. Poner el caza en el centro con velocidad (0,0) y rotaci�n 0. No
	// hace falta desactivar la entidad (no dibujarla si el juego est� parado).
	void onCollision_FighterAsteroid();


	void powerUpAction(int num);

	void finishPowerUp();

	void fighterActions(Entity* ent_);
	// Para gestionar el mensaje de que ha acabado una ronda. Desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema.
	void onRoundStart();

	// Indica si el sistema est� activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no est� activo)
	bool active_;

	
public:
	constexpr static sysId_type id = _sys_FIGHTER;
	// Reaccionar a los mensajes recibidos (llamando a m�todos correspondientes).
	void receive(const Message& m) override;

	// Crear la entidad del caza, a�adir sus componentes, asociarla con un handler
	// correspondiente, etc.
	void initSystem() override;

	void updateFighter(Vector2D pos, Vector2D vel, float rotation);

	void updateOnlyFighter2(Vector2D pos, Vector2D vel, float rotation);
	void updateOnlyFighter1(Vector2D pos, Vector2D vel, float rotation);

	

	// Si el juego est� parado no hacer nada, en otro caso actualizar la velocidad
	// del caza y moverlo como en la pr�ctica 1 (acelerar, desacelerar, etc). Adem�s, 
	// si el juego no est� parado y el jugador pulsa la tecla de disparo, enviar un
	// mensaje con las caracter�sticas f�sicas de la bala. Recuerda que se puede disparar
	// s�lo una bala cada 0.25sec.
	void update() override;

};

