#pragma once
#pragma once
#ifndef IMAGE_H
#define IMAGE_H_
#include "src/sdlutils/Texture.h"
#include "Transform.h" 
#include "Component.h"
#include "ecs.h"
#include "Manager.h"

class Manager;
struct Image : public Component {
private:
	Transform* tr_; // Consulta las caracteristicas fisicas
	Texture* tex_;	// Imagen a rederizar
	SDL_RendererFlip s = SDL_FLIP_NONE;
public:

	constexpr static cmpId_type id = IMAGE_H;
	// Constructora
	Image(Texture* tex)  : tr_(nullptr), tex_(tex)  {}
	// Destructora
	virtual ~Image() { }
	// Inicializa el componente
	void initComponent() {
		tr_ = mngr_->getComponent<Transform>(ent_);
		assert(tr_ != nullptr);
	}

	void update() {}

	// Dibuja en escena
	void render() {
		//Cuando la imagen solo tiene un frame (sin animación)
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
		tex_->render(dest, tr_->getR());
	}
};
#endif