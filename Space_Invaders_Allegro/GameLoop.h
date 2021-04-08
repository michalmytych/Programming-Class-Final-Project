#pragma once

#include "Game.h"

class GameLoop
{
public:
	GameLoop(Game* game);

	Game* game;
	
	void runPollingLoop();
};

