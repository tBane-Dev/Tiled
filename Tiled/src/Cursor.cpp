#include "Cursor.hpp"
#include "../include/Camera.hpp"
Cursor::Cursor() {

	
	_position = sf::Mouse::getPosition(*window);
	window->setView(camera->_view);
	_worldPosition = sf::Vector2i(window->mapPixelToCoords(_position));
	//_hoveredElementGUI = nullptr;

	
}

Cursor::~Cursor() {

}

void Cursor::update() {

	_position = sf::Mouse::getPosition(*window);

	window->setView(camera->_view);
	_worldPosition = sf::Vector2i(window->mapPixelToCoords(_position));
}

void Cursor::handleEvent(const sf::Event& event) {

}

void Cursor::draw() {
	sf::VertexArray vertexArray(sf::PrimitiveType::Triangles, 4);
}

std::shared_ptr<Cursor> cursor;