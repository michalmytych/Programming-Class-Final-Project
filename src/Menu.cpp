#include "Menu.h"

void Menu::runViewLoop()
{
	{
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
				clclib::Calculations::drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.05);
			}

			for (i = 0; i < 99; i++) {
				if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
					smallStarsPos[i][0] = rand() % SCALED_WIDTH;
					smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.01;
				}
				clclib::Calculations::drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.01);
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

		al_draw_bitmap(logo, (SCALED_WIDTH / 2) - al_get_bitmap_width(logo) / 2, ((SCALED_HEIGHT / 2) - al_get_bitmap_height(logo) / 2) - 70, 0);
		al_draw_text(font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 90, (SCALED_HEIGHT / 2) - 20, 0, "HIGHEST   SCORE");
		al_draw_text(score_font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 13, (SCALED_HEIGHT / 2) + 10, 0, highestScoreBuf);


		if (text_visible) {
			al_draw_text(font, al_map_rgb(147, 61, 185), (SCALED_WIDTH / 2) - 130, (SCALED_HEIGHT / 2) + 70, 0, "PRESS   ENTER   TO   PLAY !");
		}


		al_flip_display();
	}
}
