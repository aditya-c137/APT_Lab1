#include "enemy.h"
//#include <iostream>

#define WIDTH 600
#define HEIGHT 800

ECE_Enemy::ECE_Enemy(float startX, float startY, float scale, int32_t initialDir)
	:
	textureScale(scale),
	enemyDir(initialDir)
{
	enemyTexture.loadFromFile("graphics/clemson_tigers-nobg-sq.png");
	this->setTexture(enemyTexture);
	this->setScale(textureScale, textureScale);
	enemyPos.x = startX - (getGlobalBounds().width / 2);
	enemyPos.y = startY;
	this->setPosition(enemyPos);
}

void ECE_Enemy::update(Time dt)
{
	//std::cout << "MoveUpTo: " << moveUpTo << " pos.y: " << getPosition().y << std::endl;
	if ((enemyDir == -1 && enemyPos.x > 0)
		|| (enemyDir == 1 && enemyPos.x < WIDTH - getGlobalBounds().width))
	{
		enemyPos.x += enemyDir * enemySpeed * dt.asSeconds();
	}
	else if (!isMovingUp) 
	{
		isMovingUp = true;
		moveUpTo = getPosition().y - getGlobalBounds().height - moveUpOffset;
	}

	if (isMovingUp && enemyPos.y > moveUpTo)
	{
		enemyPos.y -= enemyPos.y > 50 ? enemySpeed * dt.asSeconds() : 0;
	}
	else if (isMovingUp && enemyPos.y < moveUpTo)
	{
		enemyPos.y = moveUpTo;
		enemyDir *= -1;
		isMovingUp = false;
	}

	setPosition(enemyPos);
}