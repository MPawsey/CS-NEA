#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace Window
{
	enum WindowStates;

	void Init();
	void Run();

	sf::RenderWindow& GetWindow();
	const WindowStates GetWindowState();
	const sf::Vector2u GetWindowSize();
	
	const Event<>& GetWindowClosedEvent();
}