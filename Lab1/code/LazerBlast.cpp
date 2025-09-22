#include "LazerBlast.h"

#define WIDTH 600
#define HEIGHT 800

ECE_LazerBlast::ECE_LazerBlast(float startX, float startY) 
: textureScale(0.15)
{
	lazerTexture.loadFromFile("graphics/flameLazer.png");
	setTexture(lazerTexture);
	setRotation(90);
	setScale(textureScale, textureScale + 0.25);
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
		lazerPos.y += lazerSpeed * dt.asSeconds();
	}
	setPosition(lazerPos);
}