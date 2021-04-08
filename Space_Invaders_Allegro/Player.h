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
	int moveState = 3;
	int prevMoveState = 3;
	int objSlowDown = 4;
	float maxVel = 10;
	float accel = 0.2;

	int getVelocity();
	void setVelocity(int velocity);
	
	int getHealth();
	void setHealth(int health);
};

