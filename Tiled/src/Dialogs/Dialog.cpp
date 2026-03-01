#include "../include/Dialogs/Dialog.hpp"
#include "../include/Theme.hpp"
#include "../include/Window.hpp"
#include "../include/Cursor.hpp"
#include "../include/Time.hpp"
#include "../include/Camera.hpp"
#include <iostream>

Dialog::Dialog() : Dialog(L"Dialog", sf::Vector2i(128, 128)) {
	
}

Dialog::Dialog(std::wstring title, sf::Vector2i size, sf::Vector2i position) : ElementGUI() {

	_title = title;

	_rect  = sf::IntRect(position, size);
	_titleRect = sf::IntRect(position, sf::Vector2i(size.x - 2 * dialog_border_width, dialog_title_rect_height));

	sf::Vector2i p = position + sf::Vector2i(dialog_border_width, dialog_border_width + _titleRect.size.y);
	_contentRect = sf::IntRect(p, sf::Vector2i(size.x - 2 * dialog_border_width, size.y - 2 * dialog_border_width - _titleRect.size.y));

	_titleText = std::make_unique<sf::Text>(basicFont, _title, dialog_title_font_size);
	_titleText->setFillColor(dialog_title_text_color);

	if (position == sf::Vector2i(-1, -1)) {
		sf::Vector2i pos;
		pos.x = window->getSize().x / 2 - size.x / 2;
		pos.y = window->getSize().y / 2 - size.y / 2;
		setPosition(pos);
	}
	else
		setPosition(position);

	_is_moved = false;
	_offset = sf::Vector2i(0, 0);

	_currentOnTabElement = -1;
}

Dialog::~Dialog() {

}

sf::Vector2i Dialog::getPosition() {
	return _rect.position;
}

void Dialog::setSize(sf::Vector2f size) {

}

sf::Vector2i Dialog::getSize() {
	return _rect.size;
}

sf::Vector2i Dialog::getContentPosition() {
	return _contentRect.position;
}
sf::Vector2i Dialog::getContentSize() {
	return _contentRect.size;
}

void Dialog::setPosition(sf::Vector2i position) {

	_position = position;

	_rect.position = _position;

	_titleRect.position = _position;

	_contentRect.position = _position + sf::Vector2i(dialog_border_width, dialog_border_width + _titleRect.size.y);

	sf::Vector2f pos(sf::Vector2f(_rect.position) + sf::Vector2f(dialog_border_width + 2 + (_titleRect.size.y - _titleText->getFont().getLineSpacing(dialog_title_font_size)) / 2.0f, dialog_border_width + (_titleRect.size.y - _titleText->getFont().getLineSpacing(dialog_title_font_size)) / 2.0f));
	_titleText->setPosition(pos);
}

void Dialog::clampPosition() {
	sf::Vector2i newPos = _position;
	newPos.x = std::clamp(newPos.x, 32, int(camera->_UIview.getSize().x) - getSize().x - 32);
	newPos.y = std::clamp(newPos.y, 32, int(camera->_UIview.getSize().y) - getSize().y - 32);
	setPosition(newPos);
}

void Dialog::deactivateOnTabElement() {
	if (_onTabElements.size() == 0) {
		return;
	}
		
	if (_currentOnTabElement < 0) {
		return;
	}
}


void Dialog::activateOnTabElement(int id) {

	deactivateOnTabElement();

	if (!_onTabElements.empty())
		_currentOnTabElement = id;
	else {
		_currentOnTabElement = -1;
		return;
	}
	
	if (_currentOnTabElement >= _onTabElements.size())
		_currentOnTabElement = 0;

	if (_currentOnTabElement >= 0) {
		// actions on elemens
		// ... 
	}
}

void Dialog::cursorHover() {

	if (_rect.contains(cursor->_position)) {
		ElementGUI_hovered = this->shared_from_this();
	}

}

void Dialog::handleEvent(const sf::Event& event) {


	if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>(); mbp && mbp->button == sf::Mouse::Button::Left) {
		if (ElementGUI_hovered.get() == this && _titleRect.contains(cursor->_position)) {
			_is_moved = true;
			_offset = _titleRect.position - cursor->_position;
		}

		if(_rect.contains(cursor->_position))
			ElementGUI_pressed = this->shared_from_this();
	}
	else if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>(); mbr && mbr->button == sf::Mouse::Button::Left) {
		_is_moved = false;
		if (ElementGUI_pressed.get() == this)
			ElementGUI_pressed = nullptr;
	}else if (_onTabElements.size() > 0) {
		if (const auto* kp = event.getIf<sf::Event::KeyPressed>(); kp && kp->code == sf::Keyboard::Key::Tab) {

			activateOnTabElement(_currentOnTabElement + 1);
		}
	}

}

void Dialog::update() {
	if (_is_moved) {
		setPosition(cursor->_position + _offset);
	}
	else {
		setPosition(_position);
		clampPosition();
	}
		

}

void Dialog::draw() {
	
	window->setView(camera->_UIview);

	sf::RectangleShape dialogRect(sf::Vector2f(_rect.size));
	dialogRect.setFillColor(dialog_border_color);
	dialogRect.setPosition(sf::Vector2f(_rect.position));
	window->draw(dialogRect);

	sf::RectangleShape titleRect(sf::Vector2f(_titleRect.size));
	titleRect.setFillColor(dialog_title_rect_color);

	sf::Vector2f titleRectPosition;
	titleRectPosition.x = float(_rect.position.x + dialog_border_width);
	titleRectPosition.y = float(_rect.position.y + dialog_border_width);
	titleRect.setPosition(titleRectPosition);

	window->draw(titleRect);

	window->draw(*_titleText);

	sf::Vector2f contentRectSize;
	contentRectSize.x = float(_rect.size.x - 2 * dialog_border_width);
	contentRectSize.y = float(_rect.size.y - 2 * dialog_border_width - _titleRect.size.y);

	sf::RectangleShape contentRect(contentRectSize);

	contentRect.setFillColor(dialog_content_rect_color);

	sf::Vector2f contentRectPosition;
	contentRectPosition.x = float(_rect.position.x + dialog_border_width);
	contentRectPosition.y = float(_rect.position.y + dialog_border_width + _titleRect.size.y);
	contentRect.setPosition(contentRectPosition);

	window->draw(contentRect);
}

std::vector<std::shared_ptr<Dialog>> dialogs;
