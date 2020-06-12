#pragma once


#include "MovableObject.h"


class Enemy : MovableObject {
public:
	float throttle;
	bool alive;
	int turn;
	int hitDistance;
};
