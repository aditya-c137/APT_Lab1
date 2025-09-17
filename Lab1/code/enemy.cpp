#include "enemy.h"

#define WIDTH 600
#define HEIGHT 800

ECE_Enemy::ECE_Enemy(float startX, float startY, float scale)
	:textureScale(scale)
{
	enemyTexture.loadFromFile("graphics/clemson_tigers-nobg-sq.png");
	this->setTexture(enemyTexture);
	this->setScale(textureScale, textureScale);
	enemyPos.x = startX - (getGlobalBounds().width / 2);
	enemyPos.y = startY;
	this->setPosition(enemyPos);
}

void ECE_Enemy::moveLeft()
{
}

void ECE_Enemy::stopLeft()
{
}

void ECE_Enemy::moveRight()
{
}

void ECE_Enemy::stopRight()
{
}

void ECE_Enemy::moveRowUp()
{
}

void ECE_Enemy::stopMoveUp()
{
}

void ECE_Enemy::update(Time dt)
{
}
