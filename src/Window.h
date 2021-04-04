#pragma once

#include <iostream>
#include <allegro5/allegro_font.h>

class Window
{
public:
	Window();
	~Window();
	
	const int SCALED_WIDTH		= 640;
	const int SCALED_HEIGHT		= 360;
	const int HUD_FONT_SIZE		= 25;
	const int SCORE_FONT_SIZE	= 40;
	const int LEVEL_1_START		= 150;

	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_TRANSFORM transformation;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_FONT* scoreFont = NULL;
	
	bool fullscreenMode = true;
	bool text_visible = true;
	bool running = true;
	bool menu = true;
	bool end_view = true;
	bool game_ended = false;

	void initScreenScaleTransformation();
	void setFullscreenMode();
};

