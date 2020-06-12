#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>


#include "MovableObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"
#include "Laser.h"
#include "CalcLib.h"


#define SCALED_WIDTH 640
#define SCALED_HEIGHT 360
#define FPS 65
#define HUD_FONT_SIZE 25
#define SCORE_FONT_SIZE 40
#define LEVEL_1_START 150




int starsPositions[50][2];
int smallStarsPos[100][2];
int sparksPositions[100][4];
int i = 0;
int r = 0;
float tmp = 0;
char scoreBuf[256];
char lifesBuf[256];
char highestScoreBuf[256];
bool text_visible = true;
bool running = true;
bool menu = true;
bool end_view = true;
bool game_ended = false;




int main()
{
	ALLEGRO_TRANSFORM transformation;


	FILE* highest_score_file;
	fopen_s(&highest_score_file, "hs.txt", "r+");


	if (highest_score_file == NULL) {
		printf_s("Nie znaleziono pliku hs.txt\n");
		highestScoreBuf[0] = '0';
		std::ofstream new_hs_file("hs.txt");
		new_hs_file << highestScoreBuf;
		printf_s("Utworzono nowy plik hs.txt\n");
	}
	else {
		while (!feof(highest_score_file)) {
			if (fgets(highestScoreBuf, 256, highest_score_file) == NULL) break;
		}
		fclose(highest_score_file);
	}



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


	srand(NULL);


	Player Player;
	Player.x = SCALED_WIDTH / 2;
	Player.y = SCALED_HEIGHT / 1.3;
	Player.vel = 0;
	Player.accel = 0.2;
	Player.maxVel = 10;
	Player.objSlowDown = 4;
	Player.prevMoveState = 3;
	Player.moveState = 3;
	Player.levelHardness = 200;
	Player.lifes = 100;
	Player.hitTime = 0;
	Player.score = 0;
	Player.hitDistance = 50;


	Laser Shot;
	Shot.x = Player.x;
	Shot.y = -72;
	Shot.vel = 20;


	Enemy Enemy;
	Enemy.x = SCALED_WIDTH / 3;
	Enemy.y = -70;
	Enemy.throttle = 3;
	Enemy.alive = true;
	Enemy.respawnDuration = 200;
	Enemy.hitTime = 0;
	Enemy.turn = 0;
	Enemy.hitDistance = 80;


	Laser EnemyShot;
	EnemyShot.x = Enemy.x;
	EnemyShot.y = SCALED_HEIGHT + 70;
	EnemyShot.vel = 18;
	EnemyShot.throwed = false;


	Explosion Explosion;
	Explosion.x = Enemy.x;
	Explosion.y = Enemy.y;
	Explosion.hitTime = 0;
	Explosion.throwed = false;


	MovableObject Spark;
	Spark.hitTime = 0;



	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	ALLEGRO_DISPLAY* display = al_create_display(SCALED_WIDTH, SCALED_HEIGHT);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
	ALLEGRO_BITMAP* logo = NULL;
	ALLEGRO_BITMAP* player_sprite = NULL;
	ALLEGRO_BITMAP* shot_sprite = NULL;
	ALLEGRO_BITMAP* enemy_sprite = NULL;
	ALLEGRO_BITMAP* enemy_shot_sprite = NULL;
	ALLEGRO_BITMAP* expl_sprite = NULL;
	ALLEGRO_BITMAP* heart_icon = NULL;
	ALLEGRO_BITMAP* alien_icon = NULL;
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_FONT* score_font = NULL;
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);



	const float scale_factor_x = ((float)al_get_display_width(display)) / SCALED_WIDTH;
	const float scale_factor_y = ((float)al_get_display_height(display)) / SCALED_HEIGHT;



	al_identity_transform(&transformation);
	al_scale_transform(&transformation, scale_factor_x, scale_factor_y);
	al_use_transform(&transformation);



	al_install_keyboard();
	al_install_mouse();



	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));



	logo = al_load_bitmap("assets/logo_min.png");
	shot_sprite = al_load_bitmap("assets/redshot.png");
	player_sprite = al_load_bitmap("assets/xwing.png");
	enemy_sprite = al_load_bitmap("assets/tiefighter.png");
	enemy_shot_sprite = al_load_bitmap("assets/greenshot.png");
	expl_sprite = al_load_bitmap("assets/explode.png");
	heart_icon = al_load_bitmap("assets/heart.png");
	alien_icon = al_load_bitmap("assets/alien.png");
	font = al_load_ttf_font("assets/arcadeClassic.TTF", HUD_FONT_SIZE, 0);
	score_font = al_load_ttf_font("assets/arcadeClassic.TTF", SCORE_FONT_SIZE, 0);
	assert(logo != NULL);
	assert(shot_sprite != NULL);
	assert(player_sprite != NULL);
	assert(enemy_sprite != NULL);
	assert(enemy_shot_sprite != NULL);
	assert(expl_sprite != NULL);
	assert(heart_icon != NULL);
	assert(alien_icon != NULL);
	assert(font != NULL);
	assert(score_font != NULL);


	int player_sprite_width = al_get_bitmap_width(player_sprite);
	int enemy_sprite_width = al_get_bitmap_width(enemy_sprite);



	al_start_timer(timer);



	// *** START VIEW LOOP ***
	while (menu) {

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);


		if (al_get_timer_count(timer) < 5) {
			for (i = 0; i < 50; i++) {
				starsPositions[i][0] = rand() % SCALED_WIDTH;
				starsPositions[i][1] = rand() % SCALED_HEIGHT;
			}
			for (i = 0; i < 100; i++) {
				smallStarsPos[i][0] = rand() % SCALED_WIDTH;
				smallStarsPos[i][1] = rand() % SCALED_HEIGHT;
			}
		}



		// *** START VIEW ***

		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));


			for (i = 0; i < 49; i++) {
				if (starsPositions[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {	
					starsPositions[i][0] = rand() % SCALED_WIDTH;
					starsPositions[i][1] = 0 - al_get_timer_count(timer) * 1.05;
				}
				Calculations::drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.05);
			}

			for (i = 0; i < 99; i++) {
				if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
					smallStarsPos[i][0] = rand() % SCALED_WIDTH;
					smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.01;
				}
				drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.01);
			}


			al_hide_mouse_cursor(display);


			// *** START MENU CONTROLLER ***

			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
			{
				menu = false;
				running = false;
			}


			if (al_key_down(&keyState, ALLEGRO_KEY_ENTER))
			{
				menu = false;
			}

			if (al_get_timer_count(timer) % 35 == 0) {
				text_visible = !text_visible;
			}

		}


		// *** START VIEW ***

		al_draw_bitmap(logo, (SCALED_WIDTH / 2) - al_get_bitmap_width(logo) / 2, ((SCALED_HEIGHT / 2) - al_get_bitmap_height(logo) / 2 )- 70, 0);
		al_draw_text(font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 90, (SCALED_HEIGHT / 2) - 20, 0, "HIGHEST   SCORE");
		al_draw_text(score_font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 13, (SCALED_HEIGHT / 2) + 10, 0, highestScoreBuf);


		if (text_visible) {
			al_draw_text(font, al_map_rgb(147, 61, 185), (SCALED_WIDTH / 2) - 130, (SCALED_HEIGHT / 2) + 70, 0, "PRESS   ENTER   TO   PLAY !");
		}


		al_flip_display();
	}



	al_set_timer_count(timer, 0);


	// *** GAME LOOP ***

	while (running)
	{

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);


		al_hide_mouse_cursor(display);


		Explosion.throwed = false;



		// *** GAME CONTROLLER ***

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
		{
			running = false;
		}



		if (al_key_down(&keyState, ALLEGRO_KEY_A) || al_key_down(&keyState, ALLEGRO_KEY_LEFT))
		{
			if (Player.prevMoveState == 2)
			{
				Player.vel = Player.vel / Player.objSlowDown;
			}
			Player.moveState = 1;
			if (Player.vel < Player.maxVel)
			{
				Player.vel = Player.vel + Player.accel;
			}
		}
		else if (al_key_down(&keyState, ALLEGRO_KEY_D) || al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
		{
			if (Player.prevMoveState == 1)
			{
				Player.vel = Player.vel / Player.objSlowDown;
			}
			Player.moveState = 2;
			if (Player.vel < Player.maxVel)
			{
				Player.vel = Player.vel + Player.accel;
			}
		}
		else
		{
			if (Player.vel >= 0.2)
			{
				Player.vel = Player.vel - Player.accel;
			}
			Player.moveState = 3;
		}



		if (al_key_down(&keyState, ALLEGRO_KEY_SPACE))
		{
			if (Shot.y < 0) {
				Shot.y = Player.y;
				Shot.x = Player.x;
			}
		}



		if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START)
		{
			if (!EnemyShot.throwed) {
				if (EnemyShot.y > SCALED_HEIGHT) {
					EnemyShot.y = Enemy.y + enemy_sprite_width / 2;
					EnemyShot.x = Enemy.x + enemy_sprite_width / 2;
					EnemyShot.throwed = true;
				}
			}
		}



		// *** GAME VIEW ***
		if (Player.moveState == 1)
		{
			Player.prevMoveState = 1;
			Player.x = Player.x - Player.vel;
		}
		else if (Player.moveState == 2)
		{
			Player.prevMoveState = 2;
			Player.x = Player.x + Player.vel;
		}
		else
		{
			if (Player.prevMoveState == 1)
			{
				Player.x = Player.x - Player.vel;
			}
			else if (Player.prevMoveState == 2)
			{
				Player.x = Player.x + Player.vel;
			}
			else
			{
				if (Player.prevMoveState == 1)
				{
					Player.x = Player.x - Player.vel;
				}
				else if (Player.prevMoveState == 2)
				{
					Player.x = Player.x + Player.vel;
				}
			}
		}



		if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
			Enemy.y = Enemy.y + Enemy.throttle;
		}



		if (Player.x <= 0)
		{
			Player.x = 0;
		}
		if (Player.x > (SCALED_WIDTH - player_sprite_width))
		{
			Player.x = SCALED_WIDTH - player_sprite_width;
		}



		if (al_get_timer_count(timer) < 5) {
			for (i = 0; i < 50; i++) {
				starsPositions[i][0] = rand() % SCALED_WIDTH;
				starsPositions[i][1] = rand() % SCALED_HEIGHT;
			}
			for (i = 0; i < 100; i++) {
				smallStarsPos[i][0] = rand() % SCALED_WIDTH;
				smallStarsPos[i][1] = rand() % SCALED_HEIGHT;
			}
		}



		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));



			for (i = 0; i < 49; i++) {
				if (starsPositions[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {	// jesli wyjdzie poza dolna krawedz, ustalana nowa pozycja
					starsPositions[i][0] = rand() % SCALED_WIDTH;
					starsPositions[i][1] = 0 - al_get_timer_count(timer) * 1.45;
				}
				drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.45);
			}



			for (i = 0; i < 99; i++) {
				if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
					smallStarsPos[i][0] = rand() % SCALED_WIDTH;
					smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.4;
				}
				drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.4);
			}
			al_draw_bitmap(player_sprite, Player.x, Player.y, 0);



			if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
				if (distanceCalculate(Shot.x + 40, Shot.y, Enemy.x + 60, Enemy.y + 60) < Enemy.hitDistance) {
					Explosion.x = Enemy.x;
					Explosion.y = Enemy.y;
					Enemy.alive = false;
					Player.score++;
					Enemy.x = rand() % SCALED_WIDTH;
					while (Enemy.x < enemy_sprite_width || Enemy.x > SCALED_WIDTH - enemy_sprite_width) {
						Enemy.x = rand() % SCALED_WIDTH;
					}
					Enemy.y = -80;
					Enemy.turn = (rand() % 2) - 1;
					Enemy.hitTime = al_get_timer_count(timer);
					Enemy.respawnDuration = rand() % Player.levelHardness;

					Explosion.hitTime = al_get_timer_count(timer);
					Explosion.throwed = true;

					if (Player.levelHardness > 50) {
						Player.levelHardness = Player.levelHardness - 2;
					}
				}
			}



			if (Enemy.x <= enemy_sprite_width) {
				Enemy.turn = 0;
			}
			if (Enemy.x >= SCALED_WIDTH - enemy_sprite_width) {
				Enemy.turn = 0;
			}


			if (Explosion.throwed) {
				for (int r = 0; r < 25; r++) {

					sparksPositions[r][0] = Explosion.x + 41;
					sparksPositions[r][1] = Explosion.y + 36;
					sparksPositions[r + 25][0] = Explosion.x + 40;
					sparksPositions[r + 25][1] = Explosion.y + 33;
					sparksPositions[r + 50][0] = Explosion.x + 44;
					sparksPositions[r + 50][1] = Explosion.y + 37;
					sparksPositions[r + 75][0] = Explosion.x + 37;
					sparksPositions[r + 75][1] = Explosion.y + 30;

					sparksPositions[r][2] = rand() % 6;
					sparksPositions[r][3] = rand() % 6;
					sparksPositions[r + 25][2] = rand() % 7;
					sparksPositions[r + 25][3] = -(rand() % 7);
					sparksPositions[r + 50][2] = -(rand() % 5);
					sparksPositions[r + 50][3] = rand() % 5;
					sparksPositions[r + 75][2] = -(rand() % 6);
					sparksPositions[r + 75][3] = -(rand() % 6);
				}
			}


			if (distanceCalculate(EnemyShot.x + 5, EnemyShot.y + 50, Player.x + 50, Player.y + 60) < Player.hitDistance) {
				Player.lifes--;
				Player.hitTime = al_get_timer_count(timer);
			}


			if ((Enemy.respawnDuration + Enemy.hitTime) == al_get_timer_count(timer)) {
				Enemy.alive = true;
			}


			if (Enemy.y >= SCALED_HEIGHT) {
				Enemy.alive = false;
				Player.lifes--;
				Enemy.hitTime = al_get_timer_count(timer);
				Enemy.y = 0;
			}


			if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
				Enemy.x = Enemy.x + Enemy.turn;
				al_draw_bitmap(enemy_sprite, Enemy.x, Enemy.y, 0);
			}


			if (Shot.y > 1) {
				Shot.y = Shot.y - Shot.vel;
				al_draw_bitmap(shot_sprite, Shot.x, Shot.y, 0);
				al_draw_bitmap(shot_sprite, Shot.x + player_sprite_width - 10, Shot.y, 0);
			}


			if (Enemy.y < SCALED_HEIGHT * 0.4) {
				if (EnemyShot.y > SCALED_HEIGHT || al_get_timer_count(timer) < 50) {
					EnemyShot.throwed = false;
				}
				else {
					EnemyShot.y = EnemyShot.y + EnemyShot.vel;
					al_draw_bitmap(enemy_shot_sprite, EnemyShot.x, EnemyShot.y, 0);
				}
			}


			if (Explosion.hitTime != 0) {
				if (al_get_timer_count(timer) - Explosion.hitTime < 8) {
					if (al_get_timer_count(timer) - Explosion.hitTime < 3) {
						al_draw_filled_rectangle(0, 0, SCALED_WIDTH, SCALED_HEIGHT, al_map_rgb(0,255,0));
					}
					al_draw_bitmap(expl_sprite, Explosion.x, Explosion.y + al_get_timer_count(timer) - Explosion.hitTime, 0);
				}
				else {
					Explosion.hitTime = 0;
					Explosion.x = Enemy.x;
					Explosion.y = Enemy.y;
				}

			}


			if (game_ended) {
				while (end_view) {
					ALLEGRO_EVENT event;
					al_wait_for_event(queue, &event);
					ALLEGRO_KEYBOARD_STATE keyState;
					al_get_keyboard_state(&keyState);

					al_draw_text(score_font, al_map_rgb(255, 0, 0), (SCALED_WIDTH / 2) - 97, (SCALED_HEIGHT / 2) - 60, 0, "GAME   OVER");
					al_draw_text(font, al_map_rgb(0, 0, 0), (SCALED_WIDTH / 2) - 65, (SCALED_HEIGHT / 2) - 20, 0, "your   score");
					al_draw_text(score_font, al_map_rgb(0, 0, 0), (SCALED_WIDTH / 2) - 13, (SCALED_HEIGHT / 2) + 10, 0, scoreBuf);

					if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE) || al_key_down(&keyState, ALLEGRO_KEY_ENTER))
					{
						game_ended = false;
						running = false;
						break;
					}
					al_flip_display();
				}
			}


			if (Explosion.throwed && al_get_timer_count(timer) > LEVEL_1_START) {
				Spark.hitTime = al_get_timer_count(timer);
			}
			if (al_get_timer_count(timer) - Spark.hitTime < 50) {
				for (int r = 0; r < 25; r++) {
					tmp = al_get_timer_count(timer) - Spark.hitTime;
					sparksPositions[r][0] = sparksPositions[r][0] + sparksPositions[r][2] + rand() % 2;
					sparksPositions[r][1] = sparksPositions[r][1] + sparksPositions[r][3] + rand() % 2;
					sparksPositions[r + 25][0] = sparksPositions[r + 25][0] + sparksPositions[r + 25][2] + rand() % 2;
					sparksPositions[r + 25][1] = sparksPositions[r + 25][1] + sparksPositions[r + 25][3] + rand() % 2;
					sparksPositions[r + 50][0] = sparksPositions[r + 50][0] + sparksPositions[r + 50][2] + rand() % 2;
					sparksPositions[r + 50][1] = sparksPositions[r + 50][1] + sparksPositions[r + 50][3] + rand() % 2;
					sparksPositions[r + 75][0] = sparksPositions[r + 75][0] + sparksPositions[r + 75][2] + rand() % 2;
					sparksPositions[r + 75][1] = sparksPositions[r + 75][1] + sparksPositions[r + 75][3] + rand() % 2;

					al_draw_filled_rectangle(sparksPositions[r][0], sparksPositions[r][1] + tmp,
						sparksPositions[r][0] + 1, sparksPositions[r][1] + 1 + tmp,
						al_map_rgb(240 - tmp * 2, 171 - tmp * 2, 0));
					al_draw_filled_rectangle(sparksPositions[r + 25][0], sparksPositions[r + 25][1] + tmp,
						sparksPositions[r + 25][0] + 1, sparksPositions[r + 25][1] + 1 + tmp,
						al_map_rgb(240 - tmp * 2, 171 - tmp * 2, 0));
					al_draw_filled_rectangle(sparksPositions[r + 50][0], sparksPositions[r + 50][1] + tmp,
						sparksPositions[r + 50][0] + 1, sparksPositions[r + 50][1] + 1 + tmp,
						al_map_rgb(240 - tmp * 2, 171 - tmp * 2, 0));
					al_draw_filled_rectangle(sparksPositions[r + 75][0], sparksPositions[r + 75][1] + tmp,
						sparksPositions[r + 75][0] + 1, sparksPositions[r + 75][1] + 1 + tmp,
						al_map_rgb(240 - tmp * 2, 171 - tmp * 2, 0));
				}

			}


			if (al_get_timer_count(timer) > 50) {
				if (al_get_timer_count(timer) - Player.hitTime < 3) {
					al_draw_filled_rectangle(0, 0, SCALED_WIDTH, SCALED_HEIGHT, al_map_rgb(0, 255, 0));
				}
			}


			if (Player.lifes <= 0) {
				if (!game_ended) {
					Explosion.x = Player.x;
					Explosion.y = Player.y;
					Explosion.hitTime = al_get_timer_count(timer);
					Explosion.throwed = true;
					game_ended = true;
				}
			}
			else {
				sprintf_s(scoreBuf, "%d", Player.score);
				sprintf_s(lifesBuf, "%d", Player.lifes);
				al_draw_bitmap(alien_icon, 10, SCALED_HEIGHT - 50, 0);
				al_draw_text(font, al_map_rgb(0, 255, 0), 15, SCALED_HEIGHT - 20, 0, scoreBuf);
				al_draw_bitmap(heart_icon, SCALED_WIDTH - 45, SCALED_HEIGHT - 60,0);
				al_draw_text(font, al_map_rgb(0, 255, 0), SCALED_WIDTH - 42, SCALED_HEIGHT - 20, 0, lifesBuf);
			}

			al_flip_display();


		}
	}

	FILE* highest_score_file_compare;
	fopen_s(&highest_score_file_compare, "hs.txt", "r");


	if (highest_score_file_compare == NULL) {
		printf_s("Blad dostepu do pliku\n");
	}
	else {
		while (!feof(highest_score_file_compare)) {
			if (fgets(highestScoreBuf, 256, highest_score_file_compare) == NULL) break;
		}
		int actualHighest = atoi(highestScoreBuf);
		if (Player.score > actualHighest) {
			fclose(highest_score_file_compare);
			fopen_s(&highest_score_file_compare, "hs.txt", "w");
			fprintf_s(highest_score_file_compare, "%d ", Player.score);
		}
		fclose(highest_score_file_compare);
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


	return 0;
}
