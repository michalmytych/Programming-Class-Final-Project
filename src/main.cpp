#include "Game.h"

/** REFACTOR **/
int starsPositions[50][2];
int smallStarsPos[100][2];
int sparksPositions[100][4];
int i = 0;
int r = 0;
float tmp = 0;
/** REFACTOR **/


int main()
{		
	Game* game = new Game();

	game->run();

	return 0;
}
