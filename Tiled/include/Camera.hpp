#pragma once
#include <SFML/Graphics.hpp>

class Camera {
	public:

	const static float zoomSpeed;
	const static float moveSpeed;
	const static float minZoom;
	const static float maxZoom;

	sf::Vector2f _position;
	sf::View _view;
	float _zoom;

	sf::View _UIview;

	Camera();
	~Camera();
	void zoom(float delta);

	void handleEvent(const sf::Event& event);
	void update();

};

extern std::shared_ptr<Camera> camera;