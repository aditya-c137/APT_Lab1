#include <SFML/Graphics.hpp>
#include <sstream>
#include "buzzy.h"
#include "enemy.h"

#define WIDTH	600
#define HEIGHT	800

using namespace sf;

int main()
{
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
	enemyGrid.push_back(new ECE_Enemy(100, 400, 0.2));

	bool isStartScreen = true;

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

		/*
		Update game frames
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		Time dt = clock.restart();
		buzzy.update(dt);

		window.clear();
		window.draw(spriteBackground);
		window.draw(* enemyGrid[0]);
		//window.draw(ene);
		window.draw(buzzy);
		window.display();
	}

	return 0;
}