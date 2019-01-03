#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace Window
{
	enum WindowStates;

	void Init();
	void Run();

	const WindowStates GetWindowState();
	const sf::Vector2u GetWindowSize();
	
	Event<>& GetWindowClosedEvent();
}