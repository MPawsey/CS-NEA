#include "InputManager.h"

#include <unordered_map>
#include "Window.h"


namespace InputManager
{
	std::unordered_map<sf::Keyboard::Key, Event<>*> keyPressedEvents;
	std::unordered_map<sf::Keyboard::Key, Event<>*> keyReleasedEvents;
	Event<sf::Keyboard::Key> genericKeyPressedEvent;
	Event<sf::Keyboard::Key> genericKeyReleasedEvent;

	std::unordered_map<sf::Mouse::Button, Event<>*> mousePressedEvents;
	std::unordered_map<sf::Mouse::Button, Event<>*> mouseReleasedEvents;
	Event<sf::Mouse::Button> genericMousePressedEvent;
	Event<sf::Mouse::Button> genericMouseReleasedEvent;
	Event<int> mouseScrolledEvents;

	std::vector<sf::Keyboard::Key> keysDown;
	std::vector<sf::Mouse::Button> mouseDown;

	Event<sf::Vector2i> mouseMovedEvent;
	Event<char> textEnteredEvent;


	// Calls the key pressed event
	void KeyPressed(sf::Keyboard::Key key)
	{
		// Checks to see if the key is not already pressed
		if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
		{
			// Adds the key to the keys down list and calls the key pressed events
			keysDown.push_back(key);
			if (keyPressedEvents[key])
			{
				keyPressedEvents[key]->Call();
			}
			genericKeyPressedEvent.Call(key);
		}
	}

	// Calls the key released event
	void KeyRelased(sf::Keyboard::Key key)
	{
		// Checks to see if the key is pressed
		if (auto i = std::find(keysDown.begin(), keysDown.end(), key); i != keysDown.end())
		{
			// Removes the key from the keys down list and calls the key released events
			keysDown.erase(i);
			if (keyReleasedEvents[key])
			{
				keyReleasedEvents[key]->Call();
			}
			genericKeyReleasedEvent.Call(key);
		}
	}

	// Calls the mouse pressed event
	void MousePressed(sf::Mouse::Button btn)
	{
		// Checks to see if the mouse button is not already pressed
		if (std::find(mouseDown.begin(), mouseDown.end(), btn) == mouseDown.end())
		{
			// Adds the mouse button to the mouse down list and calls the mouse pressed events
			mouseDown.push_back(btn);
			if (mousePressedEvents[btn])
			{
				mousePressedEvents[btn]->Call();
			}
			genericMousePressedEvent.Call(btn);
		}
	}

	// Calls the mouse released event
	void MouseReleased(sf::Mouse::Button btn)
	{
		// Checks to see if the mouse button is pressed
		if (auto i = std::find(mouseDown.begin(), mouseDown.end(), btn); i != mouseDown.end())
		{
			// Removes the mouse button from the mouse down list and calls the mouse released events
			mouseDown.erase(i);
			if (mouseReleasedEvents[btn])
			{
				mouseReleasedEvents[btn]->Call();
			}
			genericMouseReleasedEvent.Call(btn);
		}
	}

	// Calls the mouse scrolled event
	void MouseScrolled(int delta)
	{
		mouseScrolledEvents.Call(delta);
	}

	// Calls the mouse moved event
	void MouseMoved(int x, int y)
	{
		mouseMovedEvent.Call(sf::Vector2i{x, y});
	}

	// Calls the text entered event
	void TextEntered(sf::Uint32 character)
	{
		// Converts the character from Uint32 to char
		textEnteredEvent.Call(sf::String{character}.toAnsiString()[0]);
	}

	// Returns the key pressed event for a specific key
	Event<>& GetKeyPressedEvent(sf::Keyboard::Key key)
	{
		// Checks to see if the event exists, adding it if it doesn't
		if (keyPressedEvents[key])
		{
			return *keyPressedEvents[key];
		}
		else
		{
			keyPressedEvents[key] = new Event();
			return *keyPressedEvents[key];
		}
	}

	// Returns the key released event for a specific key
	Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key)
	{
		// Checks to see if the event exists, adding it if it doesn't
		if (keyReleasedEvents[key])
		{
			return *keyReleasedEvents[key];
		}
		else
		{
			keyReleasedEvents[key] = new Event();
			return *keyReleasedEvents[key];
		}
	}

	// Returns the key pressed event
	Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent()
	{
		return genericKeyPressedEvent;
	}

	// Returns the key released event
	Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent()
	{
		return genericKeyReleasedEvent;
	}


	// Returns the mouse pressed event for a specific button
	Event<>& GetMousePressedEvent(sf::Mouse::Button btn)
	{
		// Checks to see if the event exists, adding it if it doesn't
		if (mousePressedEvents[btn])
		{
			return *mousePressedEvents[btn];
		}
		else
		{
			mousePressedEvents[btn] = new Event();
			return *mousePressedEvents[btn];
		}
	}

	// Returns the mouse released event for a specific button
	Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn)
	{
		// Checks to see if the event exists, adding it if it doesn't
		if (mouseReleasedEvents[btn])
		{
			return *mouseReleasedEvents[btn];
		}
		else
		{
			mouseReleasedEvents[btn] = new Event();
			return *mouseReleasedEvents[btn];
		}
	}

	// Returns the mouse pressed event
	Event<sf::Mouse::Button>& GetGenericMousePressedEvent()
	{
		return genericMousePressedEvent;
	}

	// Returns the mouse released event
	Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent()
	{
		return genericMouseReleasedEvent;
	}

	// Returns the mouse scrolled event
	Event<int>& GetMouseScrolledEvent()
	{
		return mouseScrolledEvents;
	}

	// Returns the mouse moved event
	Event<sf::Vector2i>& GetMouseMovedEvent()
	{
		return mouseMovedEvent;
	}

	// Returns the text entered event
	Event<char>& GetTextEnteredEvent()
	{
		return textEnteredEvent;
	}

	// Returns whether a specific key is currently pressed
	bool IsKeyDown(sf::Keyboard::Key key)
	{
		return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
	}

	// Returns whether a specific mouse button is currently pressed
	bool IsMouseDown(sf::Mouse::Button btn)
	{
		return std::find(mouseDown.begin(), mouseDown.end(), btn) != mouseDown.end();
	}

	// Returns whether the mouse is currently in a specific view
	bool IsMouseInView(const sf::View& view)
	{
		sf::Vector2f tmp;
		return IsMouseInView(view, tmp);
	}

	// Returns whether the mouse is currently in a specific view
	// storing the mouses position in the view in viewMousePos if it is
	bool IsMouseInView(const sf::View& view, sf::Vector2f& viewMousePos)
	{
		// Gets the window instance for the program
		const sf::RenderWindow& window = Window::GetWindow();

		// Gets the mouse position and window size
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = (sf::Vector2f)mousePos;
		sf::Vector2u windowSize = Window::GetWindowSize();
		// Normalises the mouse position
		pos.x /= windowSize.x;
		pos.y /= windowSize.y;
		
		// Checks to see if the mouse position is inside the view
		if (view.getViewport().contains(pos))
		{
			// Maps the mouse position using the view and returns true
			viewMousePos = window.mapPixelToCoords(mousePos, view);
			return true;
		}
		return false;
	}

	// Returns whether a point is in a specific view
	bool IsPointInView(const sf::View& view, sf::Vector2i point)
	{
		sf::Vector2f tmp;
		return IsPointInView(view, point, tmp);
	}

	// Returns whether a point is in a specific view
	// storing the points position in the view in pointPos if it is
	bool IsPointInView(const sf::View& view, sf::Vector2i point, sf::Vector2f& pointPos)
	{
		// Gets the point and window size
		sf::Vector2f pos = (sf::Vector2f)point;
		sf::Vector2u windowSize = Window::GetWindowSize();
		// Normalises the point
		pos.x /= windowSize.x;
		pos.y /= windowSize.y;

		// Checks to see if the point is inside the view
		if (view.getViewport().contains(pos))
		{
			// Maps the point using the view and returns true
			pointPos = Window::GetWindow().mapPixelToCoords(point, view);
			return true;
		}
		return false;
	}

	// Returns the mouse position in a given view
	sf::Vector2f GetMousePosInView(const sf::View& view)
	{
		return Window::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Window::GetWindow()), view);
	}

	// Returns the mouse position in a given view with a known mouse location
	sf::Vector2f GetMousePosInView(const sf::View& view, sf::Vector2i relativeMousePos)
	{
		return Window::GetWindow().mapPixelToCoords(relativeMousePos, view);
	}
}