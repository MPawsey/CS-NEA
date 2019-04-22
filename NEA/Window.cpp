#include "Window.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "UI.h"
#include "EvolutionManager.h"
#include <vector>
#include "MapEditor.h"

namespace Window
{
	// Window variables
	sf::RenderWindow window;
	WindowStates state;
	std::vector<sf::Drawable*> lateDraws;
	sf::View defaultView;
	
	// Window events
	Event windowClosedEvent;
	Event<sf::Vector2u> windowResizedEvent;

	// Polls the events for the window gained this frame
	void PollWindowEvents()
	{
		// Stores the current event being polled
		sf::Event e;
		while (window.pollEvent(e))
		{
			// Determines what the event is and acts accordingly
			switch (e.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::Resized:
				windowResizedEvent.Call(window.getSize());
				break;
			case sf::Event::KeyPressed:
				InputManager::KeyPressed(e.key.code);
				break;
			case sf::Event::KeyReleased:
				InputManager::KeyRelased(e.key.code);
				break;
			case sf::Event::MouseButtonPressed:
				InputManager::MousePressed(e.mouseButton.button);
				break;
			case sf::Event::MouseButtonReleased:
				InputManager::MouseReleased(e.mouseButton.button);
				break;
			case sf::Event::MouseWheelScrolled:
				InputManager::MouseScrolled(e.mouseWheelScroll.delta);
				break;
			case sf::Event::MouseMoved:
				InputManager::MouseMoved(e.mouseMove.x, e.mouseMove.y);
				break;
			case sf::Event::TextEntered:
				InputManager::TextEntered(e.text.unicode);
				break;
			}
		}
	}

	// Initialises the window
	void Init()
	{
		// Gets the static variable for whether the window is initialised
		static bool initComplete = false;
		
		// Checks to see if the window has already been initialised
		if (!initComplete)
		{
			// Initialises the window
			window.create(sf::VideoMode{ 800, 600 }, "Matthew Pawsey NEA Project");
			window.setFramerateLimit(MENU_FRAMERATE);
			window.setKeyRepeatEnabled(false);
			defaultView = window.getDefaultView();

			// Sets the state of the window
			state = Menu;

			// Initialises the states for the program
			UI::Init();
			Menu::MenuManager::GetMenuManager().Init();
			Evolution::EvolutionManager::GetEvolutionManager().Init();
			Editor::MapEditor::GetMapEditor().Init();

			// Sets the initComplete flag to true
			initComplete = true;
		}
	}

	// Starts to run the program
	void Run()
	{
		// Gets the static variable for whether the window is running
		static bool running = false;

		// Checks to see if the window is already running
		if (!running)
		{
			// Sets the running flag to true
			running = true;

			// Runs until the window is closed
			while (window.isOpen())
			{
				// Polls the windows events
				PollWindowEvents();

				// Clears thw window
				window.clear();

				// Updates the program depending on the programs state
				switch (state)
				{
				case Menu:
					Menu::MenuManager::GetMenuManager().Update();
					break;
				case Evolution:
					Evolution::EvolutionManager::GetEvolutionManager().Update();
					break;
				case Editor:
					Editor::MapEditor::GetMapEditor().Update();
					break;
				}

				// Draws anything in the late draws list
				for (sf::Drawable* drawable : lateDraws)
					window.draw(*drawable);

				// Display the window
				window.display();
			}

			// Calls the window closed event
			windowClosedEvent.Call();
		}
	}

	// Sets the windows state
	void SetWindowState(WindowStates state)
	{
		state = state;
	}

	// Returns the instance of the window
	sf::RenderWindow& GetWindow()
	{
		return window;
	}

	// Returns the state of the window
	const WindowStates GetWindowState()
	{
		return state;
	}

	// Returns the size of the window
	const sf::Vector2u GetWindowSize()
	{
		return window.getSize();
	}

	// Returns the default view of the window
	const sf::View& GetDefaultWindowView()
	{
		return defaultView;
	}

	// Returns the event for when the window is closed
	Event<>& GetWindowClosedEvent()
	{
		return windowClosedEvent;
	}

	// Returns the event for when the window is resized
	Event<sf::Vector2u>& GetWindowResizedEvent()
	{
		return windowResizedEvent;
	}

	// Adds a drawable to the late draws list
	void AddToLateDraw(sf::Drawable& drawable)
	{
		// Only adds it if it currently does not exist in the list
		if (auto i = std::find(lateDraws.begin(), lateDraws.end(), &drawable); i == lateDraws.end())
			lateDraws.push_back(&drawable);
	}

	// Removes a drawable from the late draws list
	void RemoveFromLateDraw(sf::Drawable& drawable)
	{
		// Only attempts to remove it if it currently exists in the list
		if (auto i = std::find(lateDraws.begin(), lateDraws.end(), &drawable); i != lateDraws.end())
			lateDraws.erase(i);
	}
}