#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Window.hpp"

class Cursor {
public:
	sf::Vector2i _position;
	sf::Vector2i _worldPosition;

	//std::shared_ptr<ElementGUI> _hoveredElementGUI;
	
	Cursor();
	~Cursor();

	void update();
	void handleEvent(const sf::Event& event);
	void draw();
};

extern std::shared_ptr<Cursor> cursor;