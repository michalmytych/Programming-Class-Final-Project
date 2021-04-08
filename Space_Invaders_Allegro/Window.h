#pragma once
#include <iostream>
#include <allegro5/allegro_font.h>
#include "Game.h"

class Window
{
public:
	Window(Game* game);
	~Window();

	// * * * REFACTOR * * *
	int starsPositions[50][2];
	int smallStarsPos[100][2];

	Game* game;

	static const int SCALED_WIDTH;
	static const int SCALED_HEIGHT;
	static const int LEVEL_1_START;
		   
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_TRANSFORM* transformation = NULL;
	ALLEGRO_FONT* mainFont = NULL;
	ALLEGRO_FONT* scoreFont = NULL;
	ALLEGRO_BITMAP* logo = NULL;
	
	bool fullscreenMode = true;
	bool textVisible = true;
	bool running = true;
	//bool menu = true;
	bool endView = true;
	bool gameEnded = false;

	void initScreenScaleTransformation();
	void setFullscreenMode();
};

