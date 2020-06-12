#pragma once


#include "MovableObject.h"


class Player : public MovableObject {
public:
	float accel;
	float maxVel;
	int objSlowDown;
	int moveState;
	int prevMoveState;
	int levelHardness;
	int lifes;
	int score;
	int hitDistance;
};
