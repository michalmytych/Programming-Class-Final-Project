#pragma once

#include "Window.h"
#include "Menu.h"
#include "Player.h"
#include "GameLoop.h"

class Game
{
private:
	void init();
	void createWindow();
	void installDevices();
	void createEventsQueue();
	void createTimer();
	void getOrCreateHighestScoreFile();
	int saveInitAllegroAddons();
	void registerEventSources();
	void initBitmaps();
	void startTimer();	
	void runLoops();
	void saveScoreAfterGame();
	void uninstallDevices();
	void runExitTasks();
	void displayAuthorInfo();

	int FPS = 65;
	bool showMenu = true;
	bool runningLoop = true;

public:	
	Game();
	~Game();

	void endGameLoop();
	void closeMenu();

	Window* window;
	Menu* menu;
	Player* player;
	GameLoop* gameLoop;

	static ALLEGRO_EVENT_QUEUE* queue;
	static ALLEGRO_TIMER* timer;	

	static char highestScoreBuf[256];
	char scoreBuf[256];
	char lifesBuf[256];
	int levelHardness = 200;

	void run();
};

