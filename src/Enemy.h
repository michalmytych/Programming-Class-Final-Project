#pragma once
#include "IMovableObject.h"

class Enemy : public IMovableObject
{
private:
	int throttle = 3;
	bool alive = true;
	int respawnDuration = 200;
	int hitTime = 0;
	int turn = 0;
	int hitDistance = 80;
};

