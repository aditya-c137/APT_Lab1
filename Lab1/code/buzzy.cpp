#include "buzzy.h"
#include <iostream>

#define WIDTH	600
#define HEIGHT	800

ECE_Buzzy::ECE_Buzzy(float startX, float startY, float scale)
	: textureScale(scale)
{
	buzzTexture.loadFromFile("graphics/Buzzy_blue-nobg.png");
	this->setTexture(buzzTexture);
	this->setScale(textureScale, textureScale);
	buzzyPos.x = startX - (getGlobalBounds().width / 2);
	buzzyPos.y = startY;
	std::cout << startX << " "
		<< startY << " "
		<< buzzyPos.x << " "
		<< buzzyPos.y << std::endl;
	this->setPosition(buzzyPos);
	
}

Vector2f ECE_Buzzy::getPosition()
{
	return buzzyPos;
}

void ECE_Buzzy::moveLeft()
{
	buzzyGoLeft = true;
}

void ECE_Buzzy::stopLeft()
{
	buzzyGoLeft = false;
}

void ECE_Buzzy::moveRight()
{
	buzzyGoRight = true;
}

void ECE_Buzzy::stopRight()
{
	buzzyGoRight = false;
}

void ECE_Buzzy::update(Time dt)
{

	if (buzzyGoLeft)
	{
		buzzyPos.x -= buzzyPos.x > -((buzzTexture.getSize().x) * textureScale / 2) ?
			buzzySpeed * dt.asSeconds()
			: 0;
	}

	if (buzzyGoRight)
	{
		buzzyPos.x += buzzyPos.x < WIDTH - ((buzzTexture.getSize().x) * textureScale / 2) ?
			buzzySpeed * dt.asSeconds()
			: 0;
	}

	this->setPosition(buzzyPos);
}
