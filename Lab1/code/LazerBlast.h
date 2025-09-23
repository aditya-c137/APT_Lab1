#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class ECE_LazerBlast : public Sprite
{
public:
	ECE_LazerBlast(float startX, float startY, bool enemyLazer = false);
	Vector2f getPosition();
	bool isCollision();
	void disable();
	void setShot(bool fired);
	void update(Time dt);
	bool detectCollision(const Sprite& obj);

private:
	Vector2f lazerPos;
	Texture lazerTexture;
	float lazerSpeed = 700.0f;
	float textureScale;
	bool isLazerFired = false;
	bool isEnemyLazer = false;
};