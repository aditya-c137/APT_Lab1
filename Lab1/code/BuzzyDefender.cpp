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

	ECE_Buzzy buzzy(WIDTH/2, 0, 0.2);
	
	std::vector<ECE_Enemy *> enemyGrid;
	for (int j = 300; j < 800; j += 100) {
		for (int i = 0; i < 450; i += 100) {
			enemyGrid.push_back(new ECE_Enemy(i, j, 0.2, j % 200 == 0 ? -1 : 1));
		}
	}

	std::list<ECE_LazerBlast *> lazerList; //empty list of lazers
	const int MAX_LAZERS = 7; //Upper bound on maximum allowable lazers
	//even though lazerList.size is possible, this is
	//used to track the active lazers
	int currentLazers = 0;
	Clock lazerTimer;
	Time lazerDt;
	const float LAZER_TMOUT = 0.15f;

	//Enemy lazer list init
	std::list<ECE_LazerBlast *> eneLazList;
	const int MAX_ENE_LAZS = 3;
	int currentEneLazs = 0;
	Clock eneLazTimer;
	Time eneLazDt;
	const float ENE_LAZ_TMOUT = 0.25f;

	bool isStartScreen = true;
	bool gameOverScreen = false;

	while (window.isOpen()) {
		lazerDt += lazerTimer.restart();
		eneLazDt += eneLazTimer.restart();
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

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (gameOverScreen)
		{
				// Include cleanup code here
				// clear enemyGrid and recreate all the enemies, clear both lazer buffer , reset buzzy position
				eneLazList.clear();
				lazerList.clear();
				enemyGrid.clear();
				// Calculate offset based on buzzy size
				float offset = buzzy.getGlobalBounds().width / 2;
				buzzy.setPosition(WIDTH/2 - offset, 0);
				for (int j = 300; j < 800; j += 100) {
						for (int i = 0; i < 450; i += 100) {
								enemyGrid.push_back(new ECE_Enemy(i, j, 0.2, j % 200 == 0 ? -1 : 1));
						}
				}
				std::stringstream hudgos;
				hudgos << "Game Over!!! Buzzy was hit!" << std::endl
											<< "Click Enter to start over!!" << std::endl
											<< "Or Escape to exit the game." << std::endl;
				hud.setString(hudgos.str());
				FloatRect msgDims = hud.getLocalBounds();
				hud.setPosition((WIDTH - msgDims.width) / 2, (HEIGHT - msgDims.height) / 2);

				window.clear();
				window.draw(spriteBackground);
				window.draw(hud);
				window.display();
				if (Keyboard::isKeyPressed(Keyboad::Enter)) {
						isStartScreen = true;
						gameOverScreen = false;
				}
				continue;
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

		//No input but determine enemy fire anyways code will determine the input
		if (eneLazDt.asSeconds() > ENE_LAZ_TMOUT && currentEneLazs < MAX_ENE_LAZS)
		{
				// TODO: add code to choose which enemy/enemies fire
				// **ADD code here
				// 
				eneLazList.push_back(
						// TODO: add flag to choose the enemy sprite
						new ECE_LazerBlast(
								enemyGrid[0]->getPosition().x + enemyGrid[0]->getGlobalBounds().width,
								enemyGrid[0]->getPosition().y + enemyGrid[0]->getGlobalBounds().height
						)
				);
				eneLazDt = Time::Zero;
				++currentEneLazs;
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

		for (auto it = eneLazList.begin(); it != eneLazList.end(); ++it)
		{
				(*it)->update(dt);
		}

		//Handle enemy lazer to buzzy collisions
		for (auto it = eneLazList.begin(); it != eneLazList.end();)
		{
				if ((*it)->detectCollision(buzzy))
				{
						std::cout << "Buzzy hit!\n";
						//signal to cleanup everything and move to gameover screen
						gameOverScreen = true;
				}
		}

		if (gameOverScreen)
		{
				continue;
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