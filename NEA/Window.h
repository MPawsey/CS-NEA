#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace Window
{
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
	
	const Event<>& GetWindowClosedEvent();

	void AddToLateDraw(sf::Drawable& drawable);
	void RemoveFromLateDraw(sf::Drawable& drawable);
}