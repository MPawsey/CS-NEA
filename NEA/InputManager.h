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
	void MouseMoved(int x, int y);

	void TextEntered(sf::Uint32 character);

	Event<>& GetKeyPressedEvent(sf::Keyboard::Key key);
	Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key);
	Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent();
	Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent();

	Event<>& GetMousePressedEvent(sf::Mouse::Button btn);
	Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn);
	Event<sf::Mouse::Button>& GetGenericMousePressedEvent();
	Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent();
	Event<int>& GetMouseScrolledEvent();

	Event<sf::Vector2i>& GetMouseMovedEvent();
	Event<char>& GetTextEnteredEvent();

	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsMouseDown(sf::Mouse::Button btn);

	bool IsMouseInView(const sf::View& view);
	bool IsMouseInView(const sf::View& view, sf::Vector2f& mousePos);

	bool IsPointInView(const sf::View& view, sf::Vector2i point);
	bool IsPointInView(const sf::View& view, sf::Vector2i point, sf::Vector2f& pointPos);

	sf::Vector2f GetMousePosInView(const sf::View& view);
	sf::Vector2f GetMousePosInView(const sf::View& view, sf::Vector2i relativeMousePos);
}