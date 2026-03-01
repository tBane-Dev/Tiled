
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <SFML/Graphics.hpp>


#include "../include/Window.hpp"
#include "../include/Textures.hpp"
#include "../include/Theme.hpp"
#include "../include/Time.hpp"
#include "../include/Cursor.hpp"
#include "../include/Camera.hpp"
#include "../include/Shader.hpp"
#include "../include/Dialogs/Dialog.hpp"
#include "../include/Dialogs/Palette.hpp"
#include "../include/Tileset.hpp"
#include "../include/Map.hpp"

int main() {

	loadTextures();
	shader = std::make_shared<sf::Shader>(L"shr\\vertex.vert", L"shr\\water.frag");
	loadTheme();

	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(sf::Vector2u(1280, 720), 32), "Tiled");
	camera = std::make_shared<Camera>();
	cursor = std::make_shared<Cursor>();
	tileset = std::make_shared<Tileset>();
	palette = std::make_shared<Palette>();

	// time
	prevTime = currentTime; 
	currentTime = mainClock.getElapsedTime();
	deltaTime = currentTime - prevTime;
	
	mapa = std::make_shared<Map>();

	// set camera to center of map
	camera->_position = sf::Vector2f(mapa->getRect().position + mapa->getRect().size / 2);

	// init FPS clock
	sf::Clock FPSClock;
	sf::Clock FPSClockUpdate;	// clock for show FPS in main loop of Editor

	// main loop
	while (window->isOpen()) {

		prevTime = currentTime;
		currentTime = mainClock.getElapsedTime();
		deltaTime = currentTime - prevTime;

		ElementGUI_hovered = nullptr;

		cursor->update();
		mapa->cursorHover();
		palette->cursorHover();

		float FPS = 1.0f / FPSClock.restart().asSeconds();
		if (FPSClockUpdate.getElapsedTime().asSeconds() > 0.5f) {

			std::ostringstream ss;
			ss << std::fixed << std::setprecision(2) << FPS << " FPS";
			window->setTitle("Tiled - " + ss.str());
			FPSClockUpdate.restart();
		}

		// Handle screen resizes
		while (const std::optional event = window->pollEvent()) {

			if (event->is<sf::Event::Closed>()) {
				window->close();
			}

			camera->handleEvent(*event);
			mapa->handleEvent(*event);
			palette->handleEvent(*event);
		}

		// update
		camera->update();
		palette->update();

		// render
		window->clear();
		mapa->draw();
		palette->draw();
		window->display();
	}
	return 0;
}
