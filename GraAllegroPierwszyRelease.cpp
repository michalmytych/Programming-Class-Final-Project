#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
//#include <allegro5/native.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>




void setTiles(int player_location_x, int player_location_y);
void drawTiles(int tile_x_index, int tile_y_index, int player_x, int player_y);

// makra
#define SCALED_WIDTH 640
#define SCALED_HEIGHT 360
#define FPS 65
#define HUD_FONT_SIZE 25
#define SCORE_FONT_SIZE 40
#define LEVEL_1_START 150
#define MAP_SIZE_X 10
#define MAP_SIZE_Y 10
//#define BLOCK_SIZE 10


// klasa wszystkich bioracych udzial w rozgrywce obiektow:
class MovableObject {
public:
	int x;							// polozenie w osi x
	int y;							// polozenie w osi y
	float vel;						// predkosc w osi x
	float accel;					// przyspieszenie w osi x
	float throttle;					// "gaz" w osi y
	float maxVel;					// maksymalna predkosc w osi x
	int objSlowDown;				// faktor wytracania predkosci w osi x
	int moveState;					// kierunek w jakim porusza sie aktualnie obiekt 1 - lewo, 2 - prawo, 3 - zatrzymany
	int prevMoveState;				// kierunek/state w jakim poruszal sie obiekt przed zmiana kierunku 
	bool alive;						// jesli prawdziwy, obiekt jest obecny w rozgrywce
	float respawnDuration;			// czas do ponownego pojawienia sie obiektu
	float hitTime;					// timer count w ktorym obiekt skolidowal sie z innym
	int levelHardness;				// wspolczynnik trudnosci (skracany czas spawnowania sie wrogow)
	int lifes;						// ilosc punktow zywotnosci
	bool throwed;					// status pocisku, jesli true => wystrzelony
	int score;						// ilosc pokonanych przeciwnikow
	int turn;						// zwrot przeciwnika
	int hitDistance;				// dystans ponizej jakiego spotkanie z pociskiem konczy sie trafieniem
};





// ruszajaca sie mapa
int map_width;
int map_height;
int player_location_x;
int player_location_y;
int tile_indexes[2];



// mapa wczytywana na biezaco
int loadCounterX = 0, loadCounterY = 0, mapSizeX = 0, mapSizeY = 0;



void loadMap(const char* filename, int** map) {
	std::ifstream openfile(filename);
	if (openfile.is_open()) {
		openfile >> mapSizeX >> mapSizeY;
		while (!openfile.eof()) {
			openfile >> map[loadCounterX][loadCounterY];
			loadCounterX++;

			if (loadCounterX >= mapSizeX) {
				loadCounterX = 0;
				loadCounterY++;
			}
		}
	}
	else {
		printf("Nie udalo sie wczytac mapy!\n");
	}
}


// funkcja rysujaca pieciopikselowa gwiazde
void drawStar(int x, int y) {
	al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
	al_draw_pixel(x + 1, y, al_map_rgb(255, 255, 255));
	al_draw_pixel(x, y + 1, al_map_rgb(255, 255, 255));
	al_draw_pixel(x - 1, y, al_map_rgb(255, 255, 255));
	al_draw_pixel(x, y - 1, al_map_rgb(255, 255, 255));
}


// to samo dla mniejszej
void drawSmallStar(int x, int y) {
	al_draw_pixel(x, y, al_map_rgb(255, 255, 255));
}


// obliczanie dystansu miedzy pociskami i wrogiem / graczem
float distanceCalculate(float x1, float y1, float x2, float y2)
{
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}



int main()
{
	// zmienne
	bool running = true;
	bool menu = true;
	int i = 0;
	int r = 0;
	float tmp = 0;
	int starsPositions[50][2];
	int smallStarsPos[100][2];
	int sparksPositions[100][4];
	bool text_visible = true;
	char scoreBuf[256];
	char lifesBuf[256];
	char highestScoreBuf[256];
	// zmienne lokalne
	int map[100][100];


	// inicjalizacja skalowania
	ALLEGRO_TRANSFORM transformation;


	// Otwieranie pliku z najwyzszym wynikiem:
	FILE* highest_score_file;
	fopen_s(&highest_score_file, "hs.txt", "r+");


	// w przypadku bledu dostepu uzywam innej metody i tworze nowy plik
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



	// inicjalizacje modulow ze sprawdzeniem:
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


	MovableObject Player;
	Player.x = SCALED_WIDTH / 2;
	Player.y = SCALED_HEIGHT / 1.3;
	Player.vel = 0;
	Player.throttle = 1.4;
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


	MovableObject Shot;
	Shot.x = Player.x;
	Shot.y = -72;
	Shot.vel = 20;


	MovableObject Enemy;
	Enemy.x = SCALED_WIDTH / 3;
	Enemy.y = -70;
	Enemy.throttle = 3;
	Enemy.alive = true;
	Enemy.respawnDuration = 200;
	Enemy.hitTime = 0;
	Enemy.turn = 0;
	Enemy.hitDistance = 80;


	MovableObject EnemyShot;
	EnemyShot.x = Enemy.x;
	EnemyShot.y = SCALED_HEIGHT + 70;
	EnemyShot.vel = 18;
	EnemyShot.throwed = false;


	MovableObject Explosion;
	Explosion.x = Enemy.x;
	Explosion.y = Enemy.y;
	Explosion.hitTime = 0;
	Explosion.throwed = false;


	MovableObject Spark;
	Spark.hitTime = 0;


	// obiekty udostepniane przez biblioteke allegro
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
	ALLEGRO_FONT* font = NULL;
	ALLEGRO_FONT* score_font = NULL;
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);


	// stale oznaczajace faktor skalowania display-u do pelnego ekranu
	const float scale_factor_x = ((float)al_get_display_width(display)) / SCALED_WIDTH;
	const float scale_factor_y = ((float)al_get_display_height(display)) / SCALED_HEIGHT;


	// transformacja skalowania display-u
	al_identity_transform(&transformation);
	al_scale_transform(&transformation, scale_factor_x, scale_factor_y);
	al_use_transform(&transformation);


	// instalacje kontrolerow
	al_install_keyboard();
	al_install_mouse();


	// rejestrowanie zrodel event-ow
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));


	// obiekty sprite-ow
	logo = al_load_bitmap("assets/logo_min.png");
	shot_sprite = al_load_bitmap("assets/redshot.png");
	player_sprite = al_load_bitmap("assets/xwing.png");
	enemy_sprite = al_load_bitmap("assets/tiefighter.png");
	enemy_shot_sprite = al_load_bitmap("assets/greenshot.png");
	expl_sprite = al_load_bitmap("assets/explode.png");
	font = al_load_ttf_font("assets/arcadeClassic.TTF", HUD_FONT_SIZE, 0);
	score_font = al_load_ttf_font("assets/arcadeClassic.TTF", SCORE_FONT_SIZE, 0);
	assert(logo != NULL);
	assert(shot_sprite != NULL);
	assert(player_sprite != NULL);
	assert(enemy_sprite != NULL);
	assert(enemy_shot_sprite != NULL);
	assert(expl_sprite != NULL);
	assert(font != NULL);
	assert(score_font != NULL);
	// pobieram tu szerokosci bitmap, bo przydadza sie wielokrotnie
	int player_sprite_width = al_get_bitmap_width(player_sprite);
	int enemy_sprite_width = al_get_bitmap_width(enemy_sprite);


	al_start_timer(timer);


	// *** PĘTLA WIDOKU STARTOWEGO ***
	while (menu) {

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);


		// losowanie pozycji nowo pojawiajacych sie gwiazd
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



		// *** WIDOK STARTOWY ***

		// Przy kazdym tick-u timera renderuje odpowiednie elementy
		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));


			// renderowanie 50 pieciopikselowych przesuwajacych sie nieustannie gwiazd
			for (i = 0; i < 49; i++) {
				if (starsPositions[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {	// jesli wyjdzie poza dolna krawedz, ustalana nowa pozycja
					starsPositions[i][0] = rand() % SCALED_WIDTH;
					starsPositions[i][1] = 0 - al_get_timer_count(timer) * 1.05;
				}
				drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.05);
			}

			// renderowanie 100 jednopikselowych gwiazd w taki sam sposob jak wieksze
			for (i = 0; i < 99; i++) {
				if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
					smallStarsPos[i][0] = rand() % SCALED_WIDTH;
					smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.01;
				}
				drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.01);
			}


			al_hide_mouse_cursor(display);


			// *** KONTROLER WIDOKU STARTOWEGO ***


			// wyjscie z programu
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
			{
				menu = false;
				running = false;
			}


			// przejscie do rozgrywki
			if (al_key_down(&keyState, ALLEGRO_KEY_ENTER))
			{
				menu = false;
			}

			if (al_get_timer_count(timer) % 35 == 0) {
				text_visible = !text_visible;
			}

		}

		// *** WIDOK ***
		al_draw_bitmap(logo, (SCALED_WIDTH / 2) - al_get_bitmap_width(logo) / 2, ((SCALED_HEIGHT / 2) - al_get_bitmap_height(logo) / 2 )- 70, 0);
		al_draw_text(font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 90, (SCALED_HEIGHT / 2) - 20, 0, "HIGHEST   SCORE");
		al_draw_text(score_font, al_map_rgb(255, 255, 255), (SCALED_WIDTH / 2) - 13, (SCALED_HEIGHT / 2) + 10, 0, highestScoreBuf);

		if (text_visible) {
			al_draw_text(font, al_map_rgb(147, 61, 185), (SCALED_WIDTH / 2) - 130, (SCALED_HEIGHT / 2) + 70, 0, "PRESS   ENTER   TO   PLAY !");
		}


		al_flip_display();
	}



	// zerowanie timera
	al_set_timer_count(timer, 0);



	// *** PETLA GRY ***

	while (running)
	{
		// nasluchiwanie na eventy
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);

		// ukrywanie kursora
		al_hide_mouse_cursor(display);


		// zmienne ktore co okrazenie petli przyjmuja taka sama wartosc
		Explosion.throwed = false;


		// *** KONTROLER ***

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || al_key_down(&keyState, ALLEGRO_KEY_ESCAPE))
		{
			running = false;
		}


		// ruch gracza lewo - prawo
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


		// strzelanie
		if (al_key_down(&keyState, ALLEGRO_KEY_SPACE))
		{
			if (Shot.y < 0) {
				Shot.y = Player.y;
				Shot.x = Player.x;
			}
		}


		// strzal wroga
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


		// *** WIDOK ***
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


		// ruch wroga w osi Y:
		if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
			Enemy.y = Enemy.y + Enemy.throttle;
		}


		// ograniczenie Player.x w tym przedziale spowoduje, że nie bedzie on wychodzil poza obreb okna
		if (Player.x <= 0)
		{
			Player.x = 0;
		}
		if (Player.x > (SCALED_WIDTH - player_sprite_width))
		{
			Player.x = SCALED_WIDTH - player_sprite_width;
		}


		// losowanie pozycji nowo pojawiajacych sie gwiazd
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


		// Przy kazdym tick-u timera renderuje odpowiednie elementy
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			// wypelnianie tla czarnym kolorem:
			al_clear_to_color(al_map_rgba_f(0, 0, 0, 1));



			// renderowanie 50 pieciopikselowych przesuwajacych sie nieustannie gwiazd
			for (i = 0; i < 49; i++) {
				if (starsPositions[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {	// jesli wyjdzie poza dolna krawedz, ustalana nowa pozycja
					starsPositions[i][0] = rand() % SCALED_WIDTH;
					starsPositions[i][1] = 0 - al_get_timer_count(timer) * 1.45;
				}
				drawStar(starsPositions[i][0], starsPositions[i][1] + al_get_timer_count(timer) * 1.45);
			}


			// renderowanie 100 jednopikselowych gwiazd w taki sam sposob jak wieksze
			for (i = 0; i < 99; i++) {
				if (smallStarsPos[i][1] + al_get_timer_count(timer) > SCALED_HEIGHT) {
					smallStarsPos[i][0] = rand() % SCALED_WIDTH;
					smallStarsPos[i][1] = 0 - al_get_timer_count(timer) * 1.4;
				}
				drawSmallStar(smallStarsPos[i][0], smallStarsPos[i][1] + al_get_timer_count(timer) * 1.4);
			}
			al_draw_bitmap(player_sprite, Player.x, Player.y, 0);


			// sprawdzanie trafienia wroga
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
					// rozpoczynanie renderowania eksplozji
					Explosion.hitTime = al_get_timer_count(timer);
					Explosion.throwed = true;
					// zmniejszanie czasu respawnu przeciwnika za kazdym razem kiedy zostaje trafiony:
					if (Player.levelHardness > 50) {
						Player.levelHardness = Player.levelHardness - 2;
					}
				}
			}


			// ograniczanie wroga w obrebie playgroundu
			if (Enemy.x <= enemy_sprite_width) {
				Enemy.turn = 0;
			}
			if (Enemy.x >= SCALED_WIDTH - enemy_sprite_width) {
				Enemy.turn = 0;
			}


			// nadawanie pozycji startowych odlamkom z eksplozji wroga
			if (Explosion.throwed) {
				for (int r = 0; r < 25; r++) {
					// poczatkowe polozenie iskry to srodek sprite-a eksplozji
					sparksPositions[r][0] = Explosion.x + 41;
					sparksPositions[r][1] = Explosion.y + 36;
					sparksPositions[r + 25][0] = Explosion.x + 40;
					sparksPositions[r + 25][1] = Explosion.y + 33;
					sparksPositions[r + 50][0] = Explosion.x + 44;
					sparksPositions[r + 50][1] = Explosion.y + 37;
					sparksPositions[r + 75][0] = Explosion.x + 37;
					sparksPositions[r + 75][1] = Explosion.y + 30;
					// w miejscach tablicy od indeksach 2 oraz 3 przechowuje randomowo wygenerowane inty,
					// ktore nadadza kierunek ruchu odlamkow
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


			// sprawdzanie czy gracz zostal trafiony
			if (distanceCalculate(EnemyShot.x + 5, EnemyShot.y + 50, Player.x + 50, Player.y + 60) < Player.hitDistance) {
				Player.lifes--;
				Player.hitTime = al_get_timer_count(timer);
			}


			// sprawdzanie czy wrog powinien sie już zespawnowac:
			if ((Enemy.respawnDuration + Enemy.hitTime) == al_get_timer_count(timer)) {
				Enemy.alive = true;
			}


			// jesli przeciwnik przedzie przez dolna krawedz okna, bedzie ponownie spawnowany,
			// a gracz straci troche punktow zdrowia
			if (Enemy.y >= SCALED_HEIGHT) {
				Enemy.alive = false;
				Player.lifes--;
				Enemy.hitTime = al_get_timer_count(timer);
				Enemy.y = 0;
			}


			// rysowanie przeciwnika jesli ma state obecnego w rozgrywce
			if (Enemy.alive && al_get_timer_count(timer) > LEVEL_1_START) {
				Enemy.x = Enemy.x + Enemy.turn;
				al_draw_bitmap(enemy_sprite, Enemy.x, Enemy.y, 0);
			}


			// renderowanie wystrzalu gracza
			if (Shot.y > 1) {
				Shot.y = Shot.y - Shot.vel;
				al_draw_bitmap(shot_sprite, Shot.x, Shot.y, 0);
				al_draw_bitmap(shot_sprite, Shot.x + player_sprite_width - 10, Shot.y, 0);
			}



			// renderowanie wystrzalu wroga
			if (Enemy.y < SCALED_HEIGHT * 0.4) {
				if (EnemyShot.y > SCALED_HEIGHT || al_get_timer_count(timer) < 50) {
					EnemyShot.throwed = false;
				}
				else {
					EnemyShot.y = EnemyShot.y + EnemyShot.vel;
					al_draw_bitmap(enemy_shot_sprite, EnemyShot.x, EnemyShot.y, 0);
				}
			}



			// renderowaie eksplozji
			if (Explosion.hitTime != 0) {
				if (al_get_timer_count(timer) - Explosion.hitTime < 8) {
					if (al_get_timer_count(timer) - Explosion.hitTime < 3) {
						al_draw_filled_rectangle(0, 0, SCALED_WIDTH, SCALED_HEIGHT, al_map_rgba_f(1, 1, 1, 0.005));
					}
					al_draw_bitmap(expl_sprite, Explosion.x, Explosion.y + al_get_timer_count(timer) - Explosion.hitTime, 0);
				}
				else {
					Explosion.hitTime = 0;
					Explosion.x = Enemy.x;
					Explosion.y = Enemy.y;
				}

			}


			// rysowanie iskier pozostalych po wybuchu
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


			// renderowanie czerwonego ekranu kiedy zostanie trafiony gracz
			if (al_get_timer_count(timer) > 50) {
				if (al_get_timer_count(timer) - Player.hitTime < 3) {
					al_draw_filled_rectangle(0, 0, SCALED_WIDTH, SCALED_HEIGHT, al_map_rgba_f(255, 0, 0, 0.005));
				}
			}


			// Renderowanie HUD
			if (Player.lifes <= 0) {
				// renderowanie końca gry
				// endGame();
			}
			else {
				// zapisywanie int-ow score i lifes do const char *:
				sprintf_s(scoreBuf, "%d", Player.score);
				sprintf_s(lifesBuf, "%d", Player.lifes);
				al_draw_text(font, al_map_rgb(0, 255, 0), 15, SCALED_HEIGHT - 20, 0, scoreBuf);
				al_draw_text(font, al_map_rgb(0, 255, 0), SCALED_WIDTH - 42, SCALED_HEIGHT - 20, 0, lifesBuf);
			}


			// update widoku wykonywany co okrazenie petli

			setTiles(Player.x, Player.y);
			drawTiles(20, 20, Player.x, Player.y);
			al_flip_display();


		}
	}


	// Otwieranie pliku z njwyzszym wynikiem, by porownac go z wynikiem ostatniej rozgrywki
	FILE* highest_score_file_compare;
	fopen_s(&highest_score_file_compare, "hs.txt", "r");


	if (highest_score_file_compare == NULL) {
		printf_s("Blad dostepu do pliku");
	}
	else {
		while (!feof(highest_score_file_compare)) {
			if (fgets(highestScoreBuf, 256, highest_score_file_compare) == NULL) break;
		}
		// w razie gdy ostatnio otrzymany wynik jest wiekszy od aktualnego rekordu, jest zapisywany do pliku
		int actualHighest = atoi(highestScoreBuf);
		if (Player.score > actualHighest) {
			fclose(highest_score_file_compare);
			fopen_s(&highest_score_file_compare, "hs.txt", "w");
			fprintf_s(highest_score_file_compare, "%d ", Player.score);
		}
		fclose(highest_score_file_compare);
	}



	// czyszczenie pamięci z utworzonych obiektów po wyjsciu z rozgrywki
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
	al_destroy_font(font);


	printf_s(
	"Tw\242rca: Micha\210 Mytych\nElektroniczne Przetwarzanie Informacji,\nUniwersytet Jegiello\344ski\n24.05.2020\nVisual Studio 2019\nMicrosoft Visual C++");
	getchar();

	return 0;
}

/*
void drawMap(int** map, int player_x, int player_y) {



	for (int i = 0; i < mapSizeX; i++) {
		for (int j = 0; j < mapSizeY; j++) {
			if (map[i][j] == 0) {
				al_draw_filled_rectangle(i * BLOCK_SIZE, j * BLOCK_SIZE,
					i * BLOCK_SIZE + BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE, al_map_rgb(0+j*5, 0, 0));
			}
		}
	}
}*/


void setTiles(int player_location_x, int player_location_y) {
	map_width = MAP_SIZE_X * SCALED_WIDTH;
	map_height = MAP_SIZE_Y * SCALED_HEIGHT;
	if (map_width % player_location_x == 0) {
		tile_indexes[0] = map_width / player_location_x;
	}
	if (map_height % player_location_y == 0) {
		tile_indexes[1] = map_height / player_location_y;
	}
}


void drawTiles(int tile_x_index, int tile_y_index, int player_x, int player_y) {
	// rysuj tile w ktorym jest gracz
	al_draw_filled_rectangle(SCALED_WIDTH - player_x / map_width, SCALED_HEIGHT - player_y / map_height, SCALED_WIDTH - player_x / map_width,
		SCALED_HEIGHT - player_y / map_height, al_map_rgb(tile_x_index * 10, tile_y_index * 10, 100));

	// rysuj tiles ktore są sąsiadami tego w którym jest gracz
}


/*
Przedzialy bloczkow mapy
jesli w bloczku ( przedziale ) = rysuj sąsiednie bloczki
int map_width;
int map_height;
int player_location_x;
int player_location_y;
int tile_x_index;
int tile_y_index;

setTiles(){
	MAP_SIZE_X * SCALED_WIDTH = map_width;
	MAP_SIZE_Y * SCALED_HEIGHT = map_height;
	if (map_width % player_location_x == 0){
		tile_x_index = map_width / player_location_x;
	}
	if (map_height % player_location_y == 0){
		tile_y_index = map_height / player_location_y;
	}
}
drawTiles(tile_x_index, tile_y_index, player_x, player_y){
	// rysuj tile w ktorym jest gracz
	al_draw_filled_rectangle(SCALED_WIDTH - player_x/map_width, SCALED_HEIGHT - player_y/map_height, SCALED_WIDTH - player_x/map_width, 
	SCALED_HEIGHT - player_y/map_height, al_map_rgb(tile_x_index * 10, tile_y_index * 10, 100));

	// rysuj tiles ktore są sąsiadami tego w którym jest gracz
}

*/