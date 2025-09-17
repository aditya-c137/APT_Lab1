#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class ECE_Enemy : public Sprite
{
public:
	ECE_Enemy(float startX, float startY, float scale);
	void moveLeft();
	void stopLeft();
	void moveRight();
	void stopRight();
	void moveRowUp();
	void stopMoveUp(); //Is this needed?
	void update(Time dt);

private:
	float textureScale;
	Texture enemyTexture;
	Vector2f enemyPos;
};