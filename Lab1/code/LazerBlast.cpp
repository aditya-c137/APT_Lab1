#include "LazerBlast.h"
#include <iostream>

#define WIDTH 600
#define HEIGHT 800

ECE_LazerBlast::ECE_LazerBlast(float startX, float startY, bool enemyLazer) 
: textureScale(0.15), isEnemyLazer(enemyLazer)
{
	if (!isEnemyLazer) 
	{
		lazerTexture.loadFromFile("graphics/flameLazer.png");
		setTexture(lazerTexture);
		setRotation(90);
		setScale(textureScale, textureScale + 0.25);
	}
	else 
	{
		lazerTexture.loadFromFile("graphics/lazer.png");
		setTexture(lazerTexture);
		setRotation(270);
		setScale(textureScale, textureScale);
	}
	lazerPos.x = startX; // -(getGlobalBounds().width / 2);
	lazerPos.y = startY;
	setPosition(lazerPos);

	setShot(true);
}

Vector2f ECE_LazerBlast::getPosition()
{
	return lazerPos;
}

bool ECE_LazerBlast::isCollision()
{
	return false;
}

void ECE_LazerBlast::disable()
{

}

void ECE_LazerBlast::setShot(bool fired) {
	isLazerFired = true;
}

void ECE_LazerBlast::update(Time dt) {
	if (isLazerFired) {
		lazerPos.y += lazerSpeed * dt.asSeconds()
			* (isEnemyLazer ? -1 : 1);
	}
	setPosition(lazerPos);
}

bool ECE_LazerBlast::detectCollision(const Sprite& obj)
{
	float objW = obj.getGlobalBounds().width;
	float objH = obj.getGlobalBounds().height;
	float objX = obj.getPosition().x;
	float objY = obj.getPosition().y;

	float lazX = this->getPosition().x;
	float lazY = this->getPosition().y;
	float lazW = this->getGlobalBounds().width;
	float lazH = this->getGlobalBounds().height;

	//Collision checks assume rectangular colliders
	if ((lazX > objX && lazX < objX + objW)
		|| (lazX + lazW > objX && lazX + lazW < objX + objW)
		) {
		if (lazY > objY && lazY < objY + objH) {
			//std::cout << objX << "\t" << objY << "\t" << objW << "\t" << objH
			//	<< "\n" << lazX << "\t" << lazY << "\t" << lazW << "\t" << lazH
			//	<< std::endl;
			return true;
		}
		else if (lazY + lazH > objY && lazY + lazH < objY + objH)
		{
			//std::cout << objX << "\t" << objY << "\t" << objW << "\t" << objH
			//	<< "\n" << lazX << "\t" << lazY << "\t" << lazW << "\t" << lazH
			//	<< std::endl;
			return true;
		}
	}
	return false;
}
