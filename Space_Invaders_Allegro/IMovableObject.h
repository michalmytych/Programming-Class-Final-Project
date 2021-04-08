#pragma once

#include <allegro5/allegro.h>

class IMovableObject
{
public:	
	IMovableObject(const char filePath[]);
	~IMovableObject();
	
	int spriteWidth = 0;

protected:
	int x = 0;
	int y = 0;
	int velocity = 0;

	ALLEGRO_BITMAP* sprite = NULL;
};

