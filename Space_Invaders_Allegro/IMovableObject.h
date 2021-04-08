#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro.h>
#include "Window.h"

class IMovableObject
{
public:	
	IMovableObject(const char filePath[]);
	~IMovableObject();

	void setSpriteMoveState(char direction);
	void renderSprite();
	void keepSpriteInsideWindow();
	
	int spriteWidth = 0;

protected:
	int x = 0;
	int y = 0;
	int velocity = 0;
	int moveState = 3;
	int prevMoveState = 3;
	int objSlowDown = 4;
	float maxVel = 10;
	float accel = 0.2;

	ALLEGRO_BITMAP* sprite = NULL;
};

