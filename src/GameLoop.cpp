#include <stdlib.h>
#include <stdio.h>

#include "GameLoop.h"

GameLoop::GameLoop()
{

}

void GameLoop::runPollingLoop()
{
	srand(NULL);

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
			clclib::Calculations::drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.45);
		}



		for (i = 0; i < 99; i++) {
			if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
				smallStarsPos[i][0] = rand() % SCALED_WIDTH;
				smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.4;
			}
			clclib::Calculations::drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.4);
		}
		al_draw_bitmap(player_sprite, Player.x, Player.y, 0);



		if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
			if (clclib::Calculations::distanceCalculate(Shot.x + 40, Shot.y, Enemy.x + 60, Enemy.y + 60) < Enemy.hitDistance) {
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



		if (clclib::Calculations::distanceCalculate(EnemyShot.x + 5, EnemyShot.y + 50, Player.x + 50, Player.y + 60) < Player.hitDistance) {
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
					al_draw_filled_rectangle(0, 0, SCALED_WIDTH, SCALED_HEIGHT, al_map_rgb(0, 255, 0));
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
			al_draw_bitmap(heart_icon, SCALED_WIDTH - 45, SCALED_HEIGHT - 60, 0);
			al_draw_text(font, al_map_rgb(0, 255, 0), SCALED_WIDTH - 42, SCALED_HEIGHT - 20, 0, lifesBuf);
		}


		al_flip_display();


	}
}
}
