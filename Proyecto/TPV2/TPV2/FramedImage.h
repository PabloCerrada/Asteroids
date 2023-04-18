#pragma once
#ifndef FRAMEDIMAGE_H_
#define FRAMEDIMAGE_H_
#include "Component.h"
#include "ecs.h"
struct FramedImage : public Component // Component that contains the required data of an animation
{
private:
	SDL_Rect rect,src;
	int row = 0, col = 0, cont = 0, nFrames, widthFrame, heightFrame;

public:
	constexpr static cmpId_type id = FRAMEDIMAGE_H;
	FramedImage(int w, int h, int frames);
	inline int getRow() { return row; }
	inline void setRow(int newValue) { row = newValue; }
	inline int getCol() { return col; }
	inline void setCol(int newValue) { col = newValue; }
	inline int getCont() { return cont; }
	inline void setCont(int newValue) { cont = newValue; }
	inline int getWidthFrame() { return widthFrame; }
	inline int getHeightFrame() { return heightFrame; }
	inline int getNFrames() { return nFrames; }
};

#endif