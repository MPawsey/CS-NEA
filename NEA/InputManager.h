#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace InputManager
{
	// Functions called when an event happens (from the window)
	void KeyPressed(sf::Keyboard::Key key);
	void KeyRelased(sf::Keyboard::Key key);

	void MousePressed(sf::Mouse::Button btn);
	void MouseReleased(sf::Mouse::Button btn);
	void MouseScrolled(int delta);
	void MouseMoved(int x, int y);

	void TextEntered(sf::Uint32 character);

	// Event Getters
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

	// Checks if key/mouse is currently being pressed
	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsMouseDown(sf::Mouse::Button btn);

	// Finds whether the mouse position is in a given view
	bool IsMouseInView(const sf::View& view);
	// Finds whether the mouse position is in a given view, returns transformed position as mousePos
	bool IsMouseInView(const sf::View& view, sf::Vector2f& mousePos);

	// Finds whether a point is in a given view
	bool IsPointInView(const sf::View& view, sf::Vector2i point);
	// Finds whether a point is in a given view, returns transformed point as pointPos
	bool IsPointInView(const sf::View& view, sf::Vector2i point, sf::Vector2f& pointPos);

	// Returns the mouse position in a given view
	sf::Vector2f GetMousePosInView(const sf::View& view);
	// Returns the mouse position in a given view from a known location
	sf::Vector2f GetMousePosInView(const sf::View& view, sf::Vector2i relativeMousePos);
}