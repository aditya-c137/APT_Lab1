#pragma once
/*
Author: Aditya Borse
Class: ECE6122 (section A)
Last Date Modified: 09-22-2025

Description:

Declares buzzy class and required members

*/
#include <SFML/Graphics.hpp>

using namespace sf;

class ECE_Buzzy : public Sprite
{
public:
	ECE_Buzzy(float startX, float startY, float scale);
	Vector2f getPosition();
	void moveLeft();
	void moveRight();
	void stopLeft();
	void stopRight();
	void update(Time dt);

private:
	Vector2f buzzyPos;
	RectangleShape buzzyShp;
	Texture buzzTexture;
	float buzzySpeed = 500.0f;
	bool buzzyGoRight = false;
	bool buzzyGoLeft = false;
	bool buzzyShoot = false;
	float textureScale;
};