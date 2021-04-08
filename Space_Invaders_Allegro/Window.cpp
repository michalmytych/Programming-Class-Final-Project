#include "Window.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const int Window::SCALED_WIDTH = 640;

const int Window::SCALED_HEIGHT = 360;

const int Window::LEVEL_1_START = 150;

Window::Window(Game* game)
{	
	this->game = game;
	display = al_create_display(SCALED_WIDTH, SCALED_HEIGHT);	
	assert(this->display!= NULL);	
	this->logo = al_load_bitmap("/assets/logo.png") ? al_load_bitmap("/assets/logo.png") : NULL;
}

Window::~Window()
{
	al_destroy_display(this->display);	
}

void Window::initScreenScaleTransformation()
{
	const float scaleFactorX = ((float)al_get_display_width(display)) / SCALED_WIDTH;
	const float scaleFactorY= ((float)al_get_display_height(display)) / SCALED_HEIGHT;

	al_identity_transform(transformation);
	al_scale_transform(transformation, scaleFactorX, scaleFactorY);
	al_use_transform(transformation);
}

void Window::setFullscreenMode()
{
	if (this->fullscreenMode) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
}

void Window::hideCursor()
{
	al_hide_mouse_cursor(this->display);
}
