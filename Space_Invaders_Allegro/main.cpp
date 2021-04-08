#include "Game.h"

/** REFACTOR **/
int sparksPositions[100][4];
int i = 0;
int r = 0;
float tmp = 0;
/** REFACTOR **/


int main()
{		
	Game* game = new Game();

	game->run();

	delete game;

	return 0;
}
