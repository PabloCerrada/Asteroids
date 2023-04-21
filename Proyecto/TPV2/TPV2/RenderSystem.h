#pragma once
#include "System.h"
#include "GameStateMachine.h"
#include "src/sdlutils/Font.h"
#include "src/sdlutils/Texture.h"
#include "Transform.h"
#include "Health.h"
class Game;
class RenderSystem : public System {
public:
	RenderSystem();
	constexpr static sysId_type id = _sys_RENDER;
	// Reaccionar a los mensajes recibidos (llamando a métodos correspondientes).
	void receive(const Message& m) override;

	// Inicializar el sistema, etc.
	void initSystem() override;

	// - Dibujar asteroides, balas y caza (sólo si el juego no está parado).
	// - Dibujar las vidas (siempre).
	// - Dibujar los mensajes correspondientes: si el juego está parado, etc (como en
	// la práctica 1)
	void update() override;
private:
	bool active_;
	int lives;
	Texture* healthTexture;
	Texture* fighterTexture;
	Texture* bulletTexture;
	Texture* asteroidTexture;
	Texture* asteroidGoldTexture;
	Transform* fighterTransform;
	Transform* fighterTransform2;
	Font* font;
	SDL_Color s = { 255,0,0 };

	// Para gestionar los mensajes correspondientes y actualizar los atributos
	// winner_ y state_. 
	void onRoundStart();
	void onRoundOver();
	void onGameOver();
	void onGameWin();
	int winner_ = 0; // 0 - None, 1 - Asteroid, 2- Fighter
};

