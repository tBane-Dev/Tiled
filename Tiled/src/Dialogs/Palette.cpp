#include "../include/Dialogs/Palette.hpp"
#include "../include/Tileset.hpp"
#include "../include/Map.hpp"
#include "../include/Camera.hpp"
#include "../include/Theme.hpp"
#include "../include/Window.hpp"

#include "../include/DebugLog.hpp"

PaletteButton::PaletteButton(int type, sf::Vector2i position) {

	_type = type;
	_position = position;
	generateVertexArray();
}

PaletteButton::~PaletteButton() {

}

void PaletteButton::generateVertexArray() {
	_vertexArray = sf::VertexArray(sf::PrimitiveType::Triangles);

	sf::Color color = sf::Color::White;

	int i = 1;
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			Tileset::Tile tile = tileset->groups[_type]->tiles[i];
			i += 1;

			int u = (tile.x) * Tile::side + x * Tile::side % 64;
			int v = (tile.y) * Tile::side + y * Tile::side % 64;

			int xx = _position.x + x * 16;
			int yy = _position.y + y * 16;

			sf::Vertex v1(sf::Vector2f((float)xx, (float)yy), color, sf::Vector2f((float)u, (float)v));
			sf::Vertex v2(sf::Vector2f((float)xx, (float)(yy + Tile::side)), color, sf::Vector2f((float)u, (float)(v + Tile::side)));
			sf::Vertex v3(sf::Vector2f((float)(xx + Tile::side), (float)(yy + Tile::side)), color, sf::Vector2f((float)(u + Tile::side), (float)(v + Tile::side)));
			sf::Vertex v4(sf::Vector2f((float)(xx + Tile::side), (float)yy), color, sf::Vector2f((float)(u + Tile::side), (float)v));

			_vertexArray.append(v1);
			_vertexArray.append(v2);
			_vertexArray.append(v3);

			_vertexArray.append(v1);
			_vertexArray.append(v3);
			_vertexArray.append(v4);
		}
	}
}

void PaletteButton::cursorHover() {

	sf::IntRect rect(_position, sf::Vector2i(48, 48));

	if (rect.contains(cursor->_position)) {
		ElementGUI_hovered = shared_from_this();
	}
}

void PaletteButton::handleEvent(const sf::Event& event) {
	sf::IntRect rect(_position, sf::Vector2i(48, 48));
	if (rect.contains(cursor->_position)) {

		if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
			ElementGUI_pressed = this->shared_from_this();
			return;
		}
		else if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
			if (ElementGUI_pressed.get() == this) {
				palette->_selectedType = _type;
				ElementGUI_pressed = nullptr;
				return;
			}
		}
	}
}

void PaletteButton::update() {

}

void PaletteButton::draw() {
	sf::RenderStates states;
	std::shared_ptr<Texture> texture = getTexture(L"tex\\tileset.png");
	shader->setUniform("resolution", sf::Vector2f(window->getSize()));
	shader->setUniform("time", currentTime.asSeconds());
	shader->setUniform("startPos", sf::Vector2f(_position));
	states.texture = &*texture->_texture;
	states.shader = &*shader;
	
	window->draw(_vertexArray, states);

	int borderWidth = 2;
	sf::RectangleShape rect(sf::Vector2f(48 - 2* borderWidth, 48 - 2* borderWidth));
	rect.setPosition(sf::Vector2f(_position.x + borderWidth, _position.y + borderWidth));
	if(ElementGUI_hovered.get() == this) {
		rect.setFillColor(sf::Color(255,255,255,31));
		rect.setOutlineColor(sf::Color(127, 127, 127, 255));
	}
	else {
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(dialog_border_color);
	}
	
	rect.setOutlineThickness(borderWidth);
	window->draw(rect);
	
}

Palette::Palette() : Dialog(L"Palette", sf::Vector2i(178, 300), sf::Vector2i(window->getSize().x - 178 - 32, 0 + 32)) {
	generateButtons();
	_selectedType = 0;
	_brushSize = 2;
}

Palette::~Palette() {
	
}

void Palette::generateButtons() {
	_buttons.clear();

	int i = 0;
	sf::Vector2i position;
	int margin = 8;

	for (auto& type : tileset->groups) {
		position.x = getContentPosition().x + margin + (i % 3) * (48 + 8);
		position.y = getContentPosition().y + margin + (i / 3) * (48 + 8);
		_buttons.push_back(std::make_shared<PaletteButton>(i, position));
		i++;
	}
}

void Palette::setPosition(sf::Vector2i position) {
	Dialog::setPosition(position);

	sf::Vector2i pos;
	int margin = 8;

	for (int i = 0; i < _buttons.size();i++) {
		pos.x = getContentPosition().x + margin + (i % 3) * (48 + 8);
		pos.y = getContentPosition().y + margin + (i / 3) * (48 + 8);
		_buttons[i]->_position = pos;
		_buttons[i]->generateVertexArray();
	}
}

void Palette::cursorHover() {
	Dialog::cursorHover();

	for (auto& btn : _buttons) {
		btn->cursorHover();
	}

}

void Palette::handleEvent(const sf::Event& event) {
	Dialog::handleEvent(event);

	for(auto& btn : _buttons) {
		btn->handleEvent(event);
	}
}

void Palette::update() {
	Dialog::update();
}

void Palette::draw() {

	Dialog::draw();

	for (auto& btn : _buttons) {
		btn->draw();
	}
}



std::shared_ptr<Palette> palette;