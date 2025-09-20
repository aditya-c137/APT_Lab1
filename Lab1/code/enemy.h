#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class ECE_Enemy : public Sprite
{
public:
	ECE_Enemy(float startX, float startY, float scale, int32_t initialDir);
	void update(Time dt);

private:
	float textureScale;
	Texture enemyTexture;
	Vector2f enemyPos;
	float enemySpeed = 100.0f;
	//enemy directions: -1,0,1
	int32_t enemyDir = 0;
	bool isMovingUp = false;
	float moveUpTo = 0.0f;
	const float moveUpOffset = 0.25f;
};