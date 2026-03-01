#include "../include/Camera.hpp"
#include "../include/Window.hpp"
#include "../include/Time.hpp"
#include "../include/Cursor.hpp"

const float Camera::zoomSpeed = 0.1f;
const float Camera::moveSpeed = 512.0f;
const float Camera::minZoom = 0.125f;
const float Camera::maxZoom = 4.0f;

Camera::Camera() {
	_zoom = 1.0f;

	_view = sf::View();
	_view.setSize(sf::Vector2f(window->getSize()) * _zoom);
	_view.setCenter(sf::Vector2f(0,0));

	_UIview = sf::View();
	_UIview.setSize(sf::Vector2f(window->getSize()));
	_UIview.setCenter(sf::Vector2f(window->getSize()) / 2.0f);
}

Camera::~Camera() {}

void Camera::zoom(float delta) {

	sf::Vector2f mouseBeforeZoom = sf::Vector2f(cursor->_worldPosition) - _position;
	float oldZoom = _zoom;

	 _zoom = std::clamp(_zoom + delta, minZoom, maxZoom);

	sf::Vector2f mouseAfterZoom = sf::Vector2f(mouseBeforeZoom) * (_zoom / oldZoom);
	_position += (sf::Vector2f(cursor->_worldPosition) - (_position + mouseAfterZoom));
}

void Camera::handleEvent(const sf::Event& event) {
	if (const auto* ms = event.getIf<sf::Event::MouseWheelScrolled>(); ms) {
		if (event.getIf<sf::Event::MouseWheelScrolled>()->wheel == sf::Mouse::Wheel::Vertical) {
			zoom(-event.getIf<sf::Event::MouseWheelScrolled>()->delta * zoomSpeed);
		}
	}
}

void Camera::update() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		_position.y -= moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		_position.y += moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		_position.x -= moveSpeed * _zoom * deltaTime.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		_position.x += moveSpeed * _zoom * deltaTime.asSeconds();
	}

	_view.setSize(sf::Vector2f(window->getSize()) * _zoom);
	_view.setCenter(sf::Vector2f(_position));
	window->setView(_view);
}


std::shared_ptr<Camera> camera;