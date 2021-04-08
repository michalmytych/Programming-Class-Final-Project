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
