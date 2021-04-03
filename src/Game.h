#pragma once

#include "Window.h"
#include "Menu.h"
#include "Player.h"
#include "../GameLoop.h"

class Game
{
private:
	void getOrCreateHighestScoreFile();
	int saveInitAllegroAddons();
	void createEventsQueue();
	void createTimer();
	void startTimer();

	int FPS = 65;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_TIMER* timer;

	void init();
	void runLoops();
	void runExitTasks();
	void initBitmaps();
	void installDevices();
	void registerEventSources();

public:	
	Game();

	Window* window;
	Menu* menu;
	Player* player;
	GameLoop* gameLoop;

	bool showMenu = true;
	bool runningLoop = true;

	char scoreBuf[256];
	char lifesBuf[256];
	char highestScoreBuf[256];
	int levelHardness = 200;

	void run();
};

