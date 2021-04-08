#include "IMovableObject.h"

IMovableObject::IMovableObject(const char filePath[] = NULL)
{
	// @todo Add some error handling later
	ALLEGRO_BITMAP* sprite = filePath ? al_load_bitmap(filePath) : NULL;
	this->sprite = sprite;
	assert(this->sprite != NULL);
	this->spriteWidth = al_get_bitmap_width(this->sprite);
}

IMovableObject::~IMovableObject()
{
	al_destroy_bitmap(this->sprite);
}

void IMovableObject::setSpriteMoveState(char direction)
{
		if (direction == 'L') {
			if (prevMoveState == 2)
			{
				velocity = velocity / objSlowDown;
			}
			
			moveState = 1;
			
			if (velocity < maxVel)
			{
				velocity = velocity + accel;
			}
		}
		else if (direction == 'R')
		{
			if (prevMoveState == 1)
			{
				velocity = velocity / objSlowDown;
			}
	
			moveState = 2;
	
			if (velocity < maxVel)
			{
				velocity = velocity + accel;
			}
		}
		else
		{
			if (velocity >= 0.2)
			{
				velocity = velocity - accel;
			}
			
			moveState = 3;
		}
}

void IMovableObject::renderSprite()
{
	if (moveState == 1)
	{
		prevMoveState = 1;
		x = x - velocity;
	}
	else if (moveState == 2)
	{
		prevMoveState = 2;
		x = x + velocity;
	}
	else
	{
		if (prevMoveState == 1)
		{
			x = x - velocity;
		}
		else if (prevMoveState == 2)
		{
			x = x + velocity;
		}
		else
		{
			if (prevMoveState == 1)
			{
				x = x - velocity;
			}
			else if (prevMoveState == 2)
			{
				x = x + velocity;
			}
		}
	}
}

void IMovableObject::keepSpriteInsideWindow()
{
	if (x <= 0)
	{
		x = 0;
	}
	if (x > (Window::SCALED_WIDTH - spriteWidth))
	{
		x = Window::SCALED_WIDTH - spriteWidth;
	}
}
