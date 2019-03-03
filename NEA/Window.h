#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace Window
{
	constexpr unsigned int MENU_FRAMERATE = 60u;

	enum WindowStates
	{
		Menu,
		Evolution
	};

	void Init();
	void Run();

	void SetWindowState(WindowStates state);

	sf::RenderWindow& GetWindow();
	const WindowStates GetWindowState();
	const sf::Vector2u GetWindowSize();
	const sf::View& GetDefaultWindowView();
	
	Event<>& GetWindowClosedEvent();
	Event<sf::Vector2u>& GetWindowResizedEvent();

	void AddToLateDraw(sf::Drawable& drawable);
	void RemoveFromLateDraw(sf::Drawable& drawable);
}
