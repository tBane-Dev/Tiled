#pragma once
#include "../include/Dialogs/Dialog.hpp"
#include "../include/Tileset.hpp"

class PaletteButton : public ElementGUI {
public:
	int _type;
	sf::Vector2i _position;
	sf::VertexArray _vertexArray;

	PaletteButton(int type, sf::Vector2i position);
	~PaletteButton();

	void generateVertexArray();
	
	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};

class Palette : public Dialog {
public:
	int _selectedType;
	int _brushSize;
	std::vector<std::shared_ptr<PaletteButton>> _buttons;

	Palette();
	~Palette();

	void generateButtons();
	void setPosition(sf::Vector2i position);

	virtual void cursorHover();
	virtual void handleEvent(const sf::Event& event);
	virtual void update();
	virtual void draw();
};

extern std::shared_ptr<Palette> palette;
