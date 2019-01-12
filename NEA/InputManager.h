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

	const Event<>& GetKeyPressedEvent(sf::Keyboard::Key key);
	const Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key);
	const Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent();
	const Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent();

	const Event<>& GetMousePressedEvent(sf::Mouse::Button btn);
	const Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn);
	const Event<sf::Mouse::Button>& GetGenericMousePressedEvent();
	const Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent();
	const Event<int>& GetMouseScrolledEvent();

	const Event<sf::Vector2i>& GetMouseMovedEvent();
	const Event<char>& GetTextEnteredEvent();

	bool IsKeyDown(sf::Keyboard::Key key);
	bool IsMouseDown(sf::Mouse::Button btn);

	bool IsMouseInView(const sf::View& view);
	bool IsMouseInView(const sf::View& view, sf::Vector2f& mousePos);

	bool IsPointInView(const sf::View& view, sf::Vector2i point);
	bool IsPointInView(const sf::View& view, sf::Vector2i point, sf::Vector2f& pointPos);

	sf::Vector2f GetMousePosInView(const sf::View& view);
	sf::Vector2f GetMousePosInView(const sf::View& view, sf::Vector2i relativeMousePos);
}