#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace Window
{
	// The framerate for the window whenever it is in a menu
	constexpr unsigned int MENU_FRAMERATE = 60u;

	// The different states the window can be in
	enum WindowStates
	{
		Menu,
		Evolution,
		Editor
	};

	// Initialises the window
	void Init();
	// Starts to run the window
	void Run();

	// Sets the state of the window
	void SetWindowState(WindowStates newState);

	// Gets the window instance
	sf::RenderWindow& GetWindow();
	// Gets the current state of the window
	const WindowStates GetWindowState();
	// Gets the size of the window
	const sf::Vector2u GetWindowSize();
	// Gets the default view for the window
	const sf::View& GetDefaultWindowView();
	
	// Returns events for the window
	Event<>& GetWindowClosedEvent();
	Event<sf::Vector2u>& GetWindowResizedEvent();

	// Adds a drawable to the late draw list
	void AddToLateDraw(sf::Drawable& drawable);
	// Removes a drawable from the late draw list
	void RemoveFromLateDraw(sf::Drawable& drawable);
}
