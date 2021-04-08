#include "Menu.h"
#include "Game.h"
#include "Window.h"
#include "CalcLib.h"

Menu::Menu(Window* window)
{
	this->window = window;
}

void Menu::runViewLoop()
{
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(Game::queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);

		if (al_get_timer_count(Game::timer) < 5) {
			for (int i = 0; i < 50; i++) {
				this->window->starsPositions[i][0] = rand() % Window::SCALED_WIDTH;
				this->window->starsPositions[i][1] = rand() % Window::SCALED_HEIGHT;
			}
			for (int i = 0; i < 100; i++) {
				this->window->smallStarsPos[i][0] = rand() % Window::SCALED_WIDTH;
				this->window->smallStarsPos[i][1] = rand() % Window::SCALED_HEIGHT;
			}
		}

		// *** START VIEW ***

		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));


			for (int i = 0; i < 49; i++) {
				if (this->window->starsPositions[i][1] + al_get_timer_count(Game::timer) > Window::SCALED_HEIGHT) {
					this->window->starsPositions[i][0] = rand() % Window::SCALED_WIDTH;
					this->window->starsPositions[i][1] = 0 - al_get_timer_count(Game::timer) * 1.05;
				}
				clclib::Calculations::drawStar(this->window->starsPositions[i][0], 
					this->window->starsPositions[i][1] + al_get_timer_count(Game::timer) * 1.05);
			}

			for (int i = 0; i < 99; i++) {
				if (this->window->smallStarsPos[i][1] + al_get_timer_count(Game::timer) > Window::SCALED_HEIGHT) {
					this->window->smallStarsPos[i][0] = rand() % Window::SCALED_WIDTH;
					this->window->smallStarsPos[i][1] = 0 - al_get_timer_count(Game::timer) * 1.01;
				}
				clclib::Calculations::drawSmallStar(this->window->smallStarsPos[i][0], 
					this->window->smallStarsPos[i][1] + al_get_timer_count(Game::timer) * 1.01);
			}

			al_hide_mouse_cursor(this->window->display);

			// *** START MENU CONTROLLER ***

			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
			{
				this->window->game->showMenu = false;
				this->window->game->runningLoop = false;
			}

			if (al_key_down(&keyState, ALLEGRO_KEY_ENTER))
			{
				this->window->game->showMenu = false;
			}

			if (al_get_timer_count(Game::timer) % 35 == 0) {
				this->window->textVisible = !(this->window->textVisible);
			}
		}

		// *** START VIEW ***

		al_draw_bitmap(this->window->logo, (Window::SCALED_WIDTH / 2) - al_get_bitmap_width(this->window->logo) / 2,
			((Window::SCALED_HEIGHT / 2) - al_get_bitmap_height(this->window->logo) / 2) - 70, 0);
		al_draw_text(this->window->mainFont, al_map_rgb(255, 255, 255), (Window::SCALED_WIDTH / 2) - 90, (Window::SCALED_HEIGHT / 2) - 20, 0, "HIGHEST   SCORE");
		al_draw_text(this->window->scoreFont, al_map_rgb(255, 255, 255), (Window::SCALED_WIDTH / 2) - 13, (Window::SCALED_HEIGHT / 2) + 10, 0, Game::highestScoreBuf);


		if (this->window->textVisible) {
			al_draw_text(this->window->mainFont, al_map_rgb(147, 61, 185), 
				(Window::SCALED_WIDTH / 2) - 130, (Window::SCALED_HEIGHT / 2) + 70, 0, "PRESS   ENTER   TO   PLAY !");
		}

		al_flip_display();
	}
}
