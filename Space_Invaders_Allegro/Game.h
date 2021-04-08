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
	void createWindow();
	int saveInitAllegroAddons();
	void createEventsQueue();
	void createTimer();
	void startTimer();

	int FPS = 65;		

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

	static ALLEGRO_EVENT_QUEUE* queue;
	static ALLEGRO_TIMER* timer;

	bool showMenu = true;
	bool runningLoop = true;

	static char highestScoreBuf[256];
	char scoreBuf[256];
	char lifesBuf[256];
	int levelHardness = 200;

	void run();
};

