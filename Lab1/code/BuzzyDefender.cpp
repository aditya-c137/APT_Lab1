/*
Author: Aditya Borse
Class: ECE6122 (section A)
Last Date Modified: 09-22-2025

Description:

Handle main game loop, maintains and creates list of bullets and enemies
Maintains state of buzzy the main character
Handles player input
Handles game start and end conditions

*/

#include <SFML/Graphics.hpp>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <list>
#include "buzzy.h"
#include "enemy.h"
#include "LazerBlast.h"

#define WIDTH	600
#define HEIGHT	800
#define ENEMY_START_Y	600
#define ENEMY_MAX_Y		800
#define ENEMY_START_X	0
#define ENEMY_MAX_X		600
//Upper bound on maximum allowable lazers and timeout for gaps
#define MAX_LAZERS		3
#define LAZER_TMOUT		0.25f
#define MAX_ENE_LAZS	3
#define ENE_LAZ_TMOUT   0.45f

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

	//Main game clock
	Clock clock;

	ECE_Buzzy buzzy(WIDTH/2, 0, 0.2);
	
	// Instantiate all the enemies
	std::vector<ECE_Enemy *> enemyGrid;
	for (int j = ENEMY_START_Y; j < ENEMY_MAX_Y; j += 100) {
		for (int i = ENEMY_START_X; i < ENEMY_MAX_X; i += 100) {
			enemyGrid.push_back(new ECE_Enemy(i, j, 0.2, j % 200 == 0 ? -1 : 1));
		}
	}

	//empty list of lazers
	std::list<ECE_LazerBlast *> lazerList;
	//even though lazerList.size() is possible, currentLazers is
	//used to track the active lazers 
	int currentLazers = 0;
	Clock lazerTimer;
	Time lazerDt;

	//Enemy lazer list init
	std::list<ECE_LazerBlast *> eneLazList;
	int currentEneLazs = 0;
	Clock eneLazTimer;
	Time eneLazDt;

	// Flags for game screen states
	bool isStartScreen = true;
	bool gameOverScreen = false;
	bool victoryScreen = false;
	bool eneGotPast = false;

	while (window.isOpen()) {
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

		if (gameOverScreen || victoryScreen)
		{
			std::stringstream hudgos;
			std::stringstream hudmsg;
			if (gameOverScreen) 
			{
				if (eneGotPast)
				{
					hudmsg << "Game Over! Enemies got past buzzy!";
				}
				else {
					hudmsg << "Game Over!!! Buzzy was hit!";
				}
			}
			else if (victoryScreen)
			{
				hudmsg << "Victory!!! Buzzy WON!!!!!!";
			}

			hudgos << hudmsg.str() << std::endl
					<< "Click \"Enter\" to start again!!" << std::endl
					<< "Or \"Escape\" to exit the game." << std::endl;
			hud.setString(hudgos.str());
			FloatRect msgDims = hud.getLocalBounds();
			hud.setPosition((WIDTH - msgDims.width) / 2, (HEIGHT - msgDims.height) / 2);

			window.clear();
			window.draw(spriteBackground);
			window.draw(hud);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				// Reset the game state as the user requested for new game
				isStartScreen = true;
				gameOverScreen = false;
				victoryScreen = false;
				currentEneLazs = 0;
				currentLazers = 0;
				lazerDt = Time::Zero;
				eneLazDt = Time::Zero;
				for (auto it = eneLazList.begin(); it != eneLazList.end();) {
					delete* it;
					it = eneLazList.erase(it);
				}
				for (auto it = lazerList.begin(); it != lazerList.end();) {
					delete* it;
					it = lazerList.erase(it);
				}
				for (auto it = enemyGrid.begin(); it != enemyGrid.end();) {
					delete* it;
					it = enemyGrid.erase(it);
				}
				// Calculate offset based on buzzy size
				float offset = buzzy.getGlobalBounds().width / 2;
				buzzy.setPosition(WIDTH / 2 - offset, 0);
				for (int j = ENEMY_START_Y; j < ENEMY_MAX_Y; j += 100) {
					for (int i = ENEMY_START_X; i < ENEMY_MAX_X; i += 100) {
						enemyGrid.push_back(new ECE_Enemy(i, j, 0.2, j % 200 == 0 ? -1 : 1));
					}
				}
				// Reset all the clocks and counters
				lazerDt = Time::Zero;
				eneLazDt = Time::Zero;
				clock.restart();
				lazerTimer.restart();
				eneLazTimer.restart();
			}
			continue;
		}

		if (isStartScreen) {
			std::stringstream hudss;
			hudss << "BuzzyDefender!!\n\nPress \"Enter\" to start." << std::endl;
			hud.setString(hudss.str());
			FloatRect titleDims = hud.getLocalBounds();
			hud.setPosition((WIDTH - titleDims.width) / 2, (HEIGHT - titleDims.height) / 2);

			window.clear();
			window.draw(spriteBackground);
			window.draw(hud);
			window.display();
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				isStartScreen = false;
				gameOverScreen = false;
				victoryScreen = false;
				// Reset all the clocks and counters for safe measure
				lazerDt = Time::Zero;
				eneLazDt = Time::Zero;
				lazerTimer.restart();
				eneLazTimer.restart();
				clock.restart();
			}
			continue;
		}

		// Maintain times for when characters fired the last bullet
		lazerDt += lazerTimer.restart();
		eneLazDt += eneLazTimer.restart();

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

		// Fire lazer
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			if (lazerDt.asSeconds() > LAZER_TMOUT && currentLazers < MAX_LAZERS) {
				lazerList.push_back(
					new ECE_LazerBlast(
						buzzy.getPosition().x + buzzy.getGlobalBounds().width,
						buzzy.getPosition().y + buzzy.getGlobalBounds().height));
				lazerDt = Time::Zero;
				++currentLazers;
			}
		}

		//No input but determine enemy fire here
		if (!enemyGrid.empty() && eneLazDt.asSeconds() > ENE_LAZ_TMOUT && currentEneLazs < MAX_ENE_LAZS)
		{
			int index = rand() % enemyGrid.size();
			eneLazList.push_back(
				// TODO: add flag to choose the enemy sprite
				new ECE_LazerBlast(
					enemyGrid[index]->getPosition().x + enemyGrid[0]->getGlobalBounds().width / 4,
					enemyGrid[index]->getPosition().y + enemyGrid[0]->getGlobalBounds().height / 1.5f,
					true
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

		// Update both the bullets
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
		for (auto it = eneLazList.begin(); it != eneLazList.end(); ++it)
		{
				if ((*it)->detectCollision(buzzy))
				{
						//signal to cleanup everything and move to gameover screen
						gameOverScreen = true;
				}
		}

		//Handle enemy going beyond buzzy
		for (auto it = enemyGrid.begin(); it != enemyGrid.end(); ++it)
		{
			if ((*it)->getPosition().y < buzzy.getGlobalBounds().height)
			{
				//signal to cleanup everything and move to gameover screen
				gameOverScreen = true;
				eneGotPast = true;
			}
		}

		// Game victory signal here
		if (enemyGrid.empty())
			victoryScreen = true;

		// Check for game end flags
		if (gameOverScreen || victoryScreen)
		{
			continue;
		}

		// Handle collisions of buzzy bullets to enemies
		// TODO: fragile code but works, refactor later
		for (auto it = lazerList.begin(); it != lazerList.end();)
		{
			bool collisionIt = false;
			for (auto ene = enemyGrid.begin(); ene != enemyGrid.end();) {
				if ((*it)->detectCollision(**ene))
				{
					// delete the bullet and enemy if enemy is hit
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

		/*
		Draw Game frames - Also delete objects that are beyond the window 
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
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
		if (!eneLazList.empty())
		{
			for (auto it = eneLazList.begin(); it != eneLazList.end();)
			{
				window.draw(**it);

				if ((*it)->getPosition().y < 0) {
					delete* it;
					it = eneLazList.erase(it);
					--currentEneLazs;
				}
				else {
					++it;
				}
			}
		}
		window.draw(buzzy);
		window.display();
	}

	return 0;
}