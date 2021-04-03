#include "Game.h"

#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


Game::Game()
{
	// INITIALIZE OTHER VARIABLES
	window = new Window();
	player = new Player('../assets/xwing.jpg');
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
	getOrCreateHighestScoreFile();
	createEventsQueue();
	initBitmaps();
	installDevices();
	registerEventSources();	
	startTimer();
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

void Game::registerEventSources()
{
	al_register_event_source(this->queue, al_get_keyboard_event_source());
	al_register_event_source(this->queue, al_get_mouse_event_source());
	al_register_event_source(this->queue, al_get_display_event_source(this->window->display));
	al_register_event_source(this->queue, al_get_timer_event_source(timer));
}

void Game::run()
{
	this->init();
	this->runLoops();
	this->runExitTasks();
}

void Game::runLoops()
{
	this->menu = new Menu;
	
	while (this->showMenu) {
		this->menu->runViewLoop();
	}

	al_set_timer_count(this->timer, 0);

	this->gameLoop = new GameLoop;

	while (this->runningLoop)
	{
		this->gameLoop->runPollingLoop();
	}
}

void Game::runExitTasks()
{
	// run tasks you want to execute before exiting game
	FILE* highestScoreFileCompare;
	fopen_s(&highestScoreFileCompare, "hs.txt", "r");


	if (highestScoreFileCompare == NULL) {
		printf_s("Blad dostepu do pliku\n");
	}
	else {
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



	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_destroy_bitmap(player_sprite);
	al_destroy_bitmap(enemy_sprite);
	al_destroy_bitmap(shot_sprite);
	al_destroy_bitmap(enemy_shot_sprite);
	al_destroy_bitmap(expl_sprite);
	al_destroy_bitmap(heart_icon);
	al_destroy_bitmap(logo);
	al_destroy_bitmap(alien_icon);
	al_destroy_font(font);


	printf_s(
		"Tw\242rca: Micha\210 Mytych\nElektroniczne Przetwarzanie Informacji,\nUniwersytet Jegiello\344ski\n24.05.2020\nVisual Studio 2019\nMicrosoft Visual C++");
	getchar();
}