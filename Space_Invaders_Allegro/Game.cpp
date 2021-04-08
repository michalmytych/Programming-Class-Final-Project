#include "Game.h"

#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


Game::Game()
{
}

Game::~Game()
{
	al_destroy_event_queue(this->queue);
	al_destroy_timer(this->timer);
}

void Game::getOrCreateHighestScoreFile()
{
	FILE* highestScoreFile;
	fopen_s(&highestScoreFile, "hs.txt", "r+");

	if (highestScoreFile == NULL) {
		printf_s("Nie znaleziono pliku hs.txt\n");
		this->highestScoreBuf[0] = '0';
		std::ofstream newHsfile("hs.txt");
		newHsfile << this->highestScoreBuf;
		printf_s("Utworzono nowy plik hs.txt\n");
	}
	else {
		while (!feof(highestScoreFile)) {
			if (fgets(this->highestScoreBuf, 256, highestScoreFile) == NULL) break;
		}
		fclose(highestScoreFile);
	}
}

void Game::saveScoreAfterGame()
{
	FILE* highestScoreFileCompare;
	fopen_s(&highestScoreFileCompare, "hs.txt", "r");


	if (highestScoreFileCompare == NULL) { printf_s("Blad dostepu do pliku\n"); exit; }
	
	while (!feof(highestScoreFileCompare)) {
		if (fgets(this->highestScoreBuf, 256, highestScoreFileCompare) == NULL) break;
	}
	
	int actualHighest = atoi(this->highestScoreBuf);
	
	if (this->player->score > actualHighest) {
		fclose(highestScoreFileCompare);
		fopen_s(&highestScoreFileCompare, "hs.txt", "w");
		fprintf_s(highestScoreFileCompare, "%d ", this->player->score);
	}
	
	fclose(highestScoreFileCompare);
}

void Game::createWindow()
{
	window = new Window(this);
}

int Game::saveInitAllegroAddons()
{
	if (!al_init()) {
		fprintf(stderr, "Nie udalo sie zainicjalizowac Allegro5!");
		return -1;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Nie udalo sie zainicjalizowac Image Add-on!");
		return -2;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "Nie udalo sie zainicjalizowac Primitives Add-on!");
		return -3;
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "Nie udalo sie zainicjalizowac Font Add-on!");
		return -4;
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Nie udalo sie zainicjalizowac True Type Add-on!");
		return -5;
	}
}

void Game::createEventsQueue()
{
	this->queue = al_create_event_queue();
}

void Game::createTimer()
{
	this->timer = al_create_timer(1.0 / this->FPS);
}

void Game::startTimer()
{
	al_start_timer(timer);
}

void Game::init()
{
	// That order of methods calls is obligatory
	this->getOrCreateHighestScoreFile();
	this->createEventsQueue();
	this->initBitmaps();
	this->installDevices();
	this->registerEventSources();	
	this->startTimer();

	this->player = new Player("../assets/xwing.jpg");
}

void Game::initBitmaps()
{
	// Call after loading in ALLEGRO_BITMAPS
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
}

void Game::installDevices()
{
	al_install_keyboard();
	al_install_mouse();
}

void Game::uninstallDevices()
{
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

void Game::registerEventSources()
{
	al_register_event_source(this->queue, al_get_keyboard_event_source());
	al_register_event_source(this->queue, al_get_mouse_event_source());
	al_register_event_source(this->queue, al_get_display_event_source(this->window->display));
	al_register_event_source(this->queue, al_get_timer_event_source(timer));
}

void Game::displayAuthorInfo()
{
	// @todo - polish chars are still displayed not properly
	printf_s("Tw\242rca: Micha\210 Mytych\nElektroniczne Przetwarzanie Informacji,\nUniwersytet Jegiello\344ski\n24.05.2020\nVisual Studio 2019\nMicrosoft Visual C++");
	getchar();
}

void Game::run()
{
	this->init();
	this->runLoops();
	this->runExitTasks();
}

void Game::runLoops()
{
	this->menu = this->window ? new Menu(this->window) : NULL;
	
	while (this->showMenu) {
		this->menu->runViewLoop();
	}

	al_set_timer_count(this->timer, 0);

	this->gameLoop = new GameLoop(this);

	while (this->runningLoop)
	{
		this->gameLoop->runPollingLoop();
	}
}

void Game::runExitTasks()
{
	// run tasks you want to execute before exiting game
	this->saveScoreAfterGame();
	this->uninstallDevices();	
}