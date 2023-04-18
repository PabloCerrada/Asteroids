#pragma once
#include "System.h"
#include "ecs.h"
#include "Transform.h"
#include "src/utils/Collisions.h"

class Manager;
class CollisionSystem : public System
{

private:
	Transform* trFighter;
	// Para gestionar el mensaje de que ha acabado una ronda. Desactivar el sistema.
	void onRoundOver();

	// Para gestionar el mensaje de que ha empezado una ronda. Activar el sistema.
	void onRoundStart();

	void checkCollision();

	// Indica si el sistema está activo o no (modificar el valor en onRoundOver y
	// onRoundStart, y en update no hacer nada si no está activo)
	bool active_;

public:
	constexpr static sysId_type id = _sys_COLLISIONS;

	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m) override;

	// Inicializar el sistema, etc.
	void initSystem() override;
	
	// Si el juego está parado no hacer nada, en otro caso comprobar colisiones como
	// en la práctica 1 y enviar mensajes correspondientes.
	void update() override;

};

