#include "Window.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


Window::Window()
{	
	display = al_create_display(SCALED_WIDTH, SCALED_HEIGHT);
	font = al_load_ttf_font("assets/arcadeClassic.TTF", HUD_FONT_SIZE, 0);
	scoreFont = al_load_ttf_font("assets/arcadeClassic.TTF", SCORE_FONT_SIZE, 0);
	
	assert(this->display!= NULL);
	assert(this->font != NULL);
	assert(this->scoreFont!= NULL);
}

Window::~Window()
{
	al_destroy_display(this->display);
	al_destroy_font(this->font);
	al_destroy_font(this->scoreFont);
}

void Window::initScreenScaleTransformation()
{
	const float scale_factor_x = ((float)al_get_display_width(display)) / SCALED_WIDTH;
	const float scale_factor_y = ((float)al_get_display_height(display)) / SCALED_HEIGHT;

	al_identity_transform(&transformation);
	al_scale_transform(&transformation, scale_factor_x, scale_factor_y);
	al_use_transform(&transformation);
}

void Window::setFullscreenMode()
{
	if (this->fullscreenMode) al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
}
