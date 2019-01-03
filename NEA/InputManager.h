#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace InputManager
{
	void KeyPressed(sf::Keyboard::Key key);
	void KeyRelased(sf::Keyboard::Key key);

	void MousePressed(sf::Mouse::Button btn);
	void MouseReleased(sf::Mouse::Button btn);
	void MouseScrolled(int delta);

	Event<>& GetKeyPressedEvent(sf::Keyboard::Key key);
	Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key);
	Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent();
	Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent();

	Event<>& GetMousePressedEvent(sf::Mouse::Button btn);
	Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn);
	Event<sf::Mouse::Button>& GetGenericMousePressedEvent();
	Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent();
	Event<int>& GetMouseScrolledEvent();

	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsMouseDown(sf::Mouse::Button btn);
	bool IsMouseInView(const sf::View& view);
	bool IsMouseInView(const sf::View& view, sf::Vector2f& mousePos);
}