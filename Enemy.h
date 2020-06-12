#pragma once


#include "MovableObject.h"


class Enemy : public MovableObject {
public:
	float throttle;
	bool alive;
	int turn;
	int hitDistance;
	int respawnDuration;
};
