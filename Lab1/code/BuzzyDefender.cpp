#include <SFML/Graphics.hpp>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "buzzy.h"
#include "enemy.h"
#include "LazerBlast.h"
#include <iostream>

#define WIDTH	600
#define HEIGHT	800

using namespace sf;

int main()
{
	srand(time(NULL)); // initialize seed

	VideoMode vm(WIDTH, HEIGHT);

	RenderWindow window(vm, "BuzzyDefender",
		Style::Titlebar | Style::Close);

	Texture textureBackground;
	textureBackground.loadFromFile("graphics/gameBG.jpg");

	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	Text hud;
	Font uiFont;
	uiFont.loadFromFile("fonts/KOMIKAP_.ttf");
	hud.setFont(uiFont);
	hud.setCharacterSize(25);
	hud.setFillColor(Color::White);

	Clock clock;
	Clock lazerTimer;
	Time lazerDt;
	const float LAZER_TMOUT = 0.15f;

	ECE_Buzzy buzzy(WIDTH/2, 0, 0.2);
	
	std::vector<ECE_Enemy *> enemyGrid;
	for (int j = 300; j < 800; j += 100) {
		for (int i = 0; i < 450; i += 100) {
			enemyGrid.push_back(new ECE_Enemy(i, j, 0.2, j % 200 == 0 ? -1 : 1));
		}
	}

	std::list<ECE_LazerBlast *> lazerList; //empty list of lazers
	const int MAX_LAZERS = 7; //Upper bound on maximum allowable lazers
	int currentLazers = 0; //even though lazerList.size is possible, this is 
	// used to track the active lazers

	bool isStartScreen = true;

	while (window.isOpen()) {
		lazerDt += lazerTimer.restart();
		/*
		Handle the player input
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		Event ev;
		while (window.pollEvent(ev)) {
			switch (ev.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		if (isStartScreen) {
			std::stringstream hudss;
			hudss << "BuzzyDefender!!" << std::endl;
			hud.setString(hudss.str());
			FloatRect titleDims = hud.getLocalBounds();
			hud.setPosition((WIDTH - titleDims.width) / 2, (HEIGHT - titleDims.height) / 2);

			window.clear();
			window.draw(spriteBackground);
			window.draw(hud);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				isStartScreen = false;
			}
			continue;
		}


		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			buzzy.moveLeft();
		}
		else 
		{
			buzzy.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			buzzy.moveRight();
		}
		else
		{
			buzzy.stopRight();
		}

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (lazerDt.asSeconds() > LAZER_TMOUT && currentLazers < MAX_LAZERS) {
				lazerList.push_back(
					new ECE_LazerBlast(
						buzzy.getPosition().x + buzzy.getGlobalBounds().width,
						buzzy.getPosition().y + buzzy.getGlobalBounds().height));
				/*std::cout << buzzy.getPosition().x + (buzzy.getGlobalBounds().width) << " "
					<< buzzy.getPosition().y + buzzy.getGlobalBounds().height << " "
					<< buzzy.getPosition().x << " "
					<< buzzy.getPosition().y << std::endl;*/
				lazerDt = Time::Zero;
				++currentLazers;
			}
		}

		/*
		Update game frames
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		Time dt = clock.restart();
		buzzy.update(dt);
		for (int i = 0; i < enemyGrid.size(); i++) {
			enemyGrid[i]->update(dt);
		}
		//for (const ECE_Enemy* enemy : enemyGrid) {

		//}
		if (!lazerList.empty()) {
			for (auto it = lazerList.begin(); it != lazerList.end();++it)
			{
				(*it)->update(dt);
			}
		}

		// Handle collisions
		for (auto it = lazerList.begin(); it != lazerList.end();)
		{
		//for (auto it = lazerList.begin(); it != lazerList.end(); ++it) {
			bool collisionIt = false;
			for (auto ene = enemyGrid.begin(); ene != enemyGrid.end();) {
				bool collision = (*it)->detectCollision(**ene);
				if (collision)
				{
					std::cout << "Enemy hit!!" << std::endl;
					delete* ene;
					delete* it;
					it = lazerList.erase(it);
					collisionIt = true;
					--currentLazers;
					if (ene != enemyGrid.end() - 1)
					{
						ene = enemyGrid.erase(ene);
					}
					else {
						enemyGrid.erase(ene);
						break;
					}
					break;
				}
				else {
					++ene;
				}
			}
			if (!collisionIt && !lazerList.empty())
			{
				++it;
			}
			else if (lazerList.empty()) {
				break;
			}
		}

		// Draw frames
		window.clear();
		window.draw(spriteBackground);
		for (int i = 0; i < enemyGrid.size(); i++) {
			window.draw(*enemyGrid[i]);
		}

		if (!lazerList.empty()) 
		{
			for (auto it = lazerList.begin(); it != lazerList.end();) 
			{
				window.draw(**it);

				if ((*it)->getPosition().y > HEIGHT) {
					delete* it;
					it = lazerList.erase(it);
					--currentLazers;
				}
				else {
					++it;
				}
			}
		}
		//window.draw(ene);
		window.draw(buzzy);
		window.display();
	}

	return 0;
}