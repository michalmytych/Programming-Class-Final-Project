#pragma once
#include "IMovableObject.h"

class Player : public IMovableObject
{
private:	
	int health = 100;	
	int hitDistance = 50;

public:
	Player(const char filePath[]);

	int score = 0;			
	
	int getHealth();
	void setHealth(int health);
};

