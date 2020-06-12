#pragma once


#include "MovableObject.h"


class Player : MovableObject {
public:
	float accel;
	float maxVel;
	int objSlowDown;
	int moveState;
	int prevMoveState;



};
