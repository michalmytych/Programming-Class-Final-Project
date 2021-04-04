#pragma once

#include "Window.h"
#include "Menu.h"
#include "Player.h"
#include "GameLoop.h"

class Game
{
private:
	void getOrCreateHighestScoreFile();
	void saveScoreAfterGame();
	int saveInitAllegroAddons();
	void createEventsQueue();
	void createTimer();
	void startTimer();

	int FPS = 65;	
	ALLEGRO_TIMER* timer;

	void init();
	void runLoops();
	void runExitTasks();
	void initBitmaps();
	void installDevices();
	void uninstallDevices();
	void registerEventSources();
	void displayAuthorInfo();

public:	
	Game();
	~Game();

	Window* window;
	Menu* menu;
	Player* player;
	GameLoop* gameLoop;

	ALLEGRO_EVENT_QUEUE* queue;

	bool showMenu = true;
	bool runningLoop = true;

	char scoreBuf[256];
	char lifesBuf[256];
	char highestScoreBuf[256];
	int levelHardness = 200;

	void run();
};

