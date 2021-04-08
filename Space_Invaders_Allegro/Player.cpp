#include "Player.h"

Player::Player(const char filePath[]) : IMovableObject(filePath)
{
}

int Player::getVelocity()
{
    return this->velocity;
}

void Player::setVelocity(int velocity)
{
    this->velocity = velocity;
}

int Player::getHealth()
{
    return this->health;
}

void Player::setHealth(int health)
{
    this->health = health;
}
