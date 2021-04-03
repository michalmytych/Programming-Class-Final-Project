#pragma once
#include "IMovableObject.h"

class Player : public IMovableObject
{
private:
	float accel = 0.2;
	float maxVel = 10;
	int objSlowDown = 4;
	int moveState = 3;
	int prevMoveState = 3;
	int lifes = 100;	
	int hitDistance = 50;

public:
	int score = 0;
};

