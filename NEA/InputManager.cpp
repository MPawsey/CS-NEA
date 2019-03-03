#include "InputManager.h"

#include <unordered_map>
#include "Window.h"


namespace InputManager
{
	// Private
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

	// Public

	void KeyPressed(sf::Keyboard::Key key)
	{
		if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
		{
			keysDown.push_back(key);
			if (keyPressedEvents[key])
			{
				keyPressedEvents[key]->Call();
			}
			genericKeyPressedEvent.Call(key);
		}
	}

	void KeyRelased(sf::Keyboard::Key key)
	{
		if (auto i = std::find(keysDown.begin(), keysDown.end(), key); i != keysDown.end())
		{
			keysDown.erase(i);
			if (keyReleasedEvents[key])
			{
				keyReleasedEvents[key]->Call();
			}
			genericKeyReleasedEvent.Call(key);
		}
	}

	void MousePressed(sf::Mouse::Button btn)
	{
		if (std::find(mouseDown.begin(), mouseDown.end(), btn) == mouseDown.end())
		{
			mouseDown.push_back(btn);
			if (mousePressedEvents[btn])
			{
				mousePressedEvents[btn]->Call();
			}
			genericMousePressedEvent.Call(btn);
		}
	}

	void MouseReleased(sf::Mouse::Button btn)
	{
		if (auto i = std::find(mouseDown.begin(), mouseDown.end(), btn); i != mouseDown.end())
		{
			mouseDown.erase(i);
			if (mouseReleasedEvents[btn])
			{
				mouseReleasedEvents[btn]->Call();
			}
			genericMouseReleasedEvent.Call(btn);
		}
	}

	void MouseScrolled(int delta)
	{
		mouseScrolledEvents.Call(delta);
	}

	void MouseMoved(int x, int y)
	{
		mouseMovedEvent.Call(sf::Vector2i{x, y});
	}

	void TextEntered(sf::Uint32 character)
	{
		textEnteredEvent.Call(sf::String{character}.toAnsiString()[0]);
	}

	Event<>& GetKeyPressedEvent(sf::Keyboard::Key key)
	{
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

	Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key)
	{
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

	Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent()
	{
		return genericKeyPressedEvent;
	}

	Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent()
	{
		return genericKeyReleasedEvent;
	}


	Event<>& GetMousePressedEvent(sf::Mouse::Button btn)
	{
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

	Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn)
	{
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

	Event<sf::Mouse::Button>& GetGenericMousePressedEvent()
	{
		return genericMousePressedEvent;
	}

	Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent()
	{
		return genericMouseReleasedEvent;
	}

	Event<int>& GetMouseScrolledEvent()
	{
		return mouseScrolledEvents;
	}

	Event<sf::Vector2i>& GetMouseMovedEvent()
	{
		return mouseMovedEvent;
	}

	Event<char>& GetTextEnteredEvent()
	{
		return textEnteredEvent;
	}


	bool IsKeyDown(sf::Keyboard::Key key)
	{
		return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
	}

	bool IsMouseDown(sf::Mouse::Button btn)
	{
		return std::find(mouseDown.begin(), mouseDown.end(), btn) != mouseDown.end();
	}

	bool IsMouseInView(const sf::View& view)
	{
		sf::Vector2f tmp;
		return IsMouseInView(view, tmp);
	}

	bool IsMouseInView(const sf::View& view, sf::Vector2f& viewMousePos)
	{
		const sf::RenderWindow& window = Window::GetWindow();
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = (sf::Vector2f)mousePos;
		sf::Vector2u windowSize = Window::GetWindowSize();
		pos.x /= windowSize.x;
		pos.y /= windowSize.y;
		
		if (view.getViewport().contains(pos))
		{
			viewMousePos = window.mapPixelToCoords(mousePos, view);
			return true;
		}
		return false;
	}

	bool IsPointInView(const sf::View& view, sf::Vector2i point)
	{
		sf::Vector2f tmp;
		return IsPointInView(view, point, tmp);
	}

	bool IsPointInView(const sf::View& view, sf::Vector2i point, sf::Vector2f& pointPos)
	{
		sf::Vector2f pos = (sf::Vector2f)point;
		sf::Vector2u windowSize = Window::GetWindowSize();
		pos.x /= windowSize.x;
		pos.y /= windowSize.y;

		if (view.getViewport().contains(pos))
		{
			pointPos = Window::GetWindow().mapPixelToCoords(point, view);
			return true;
		}
		return false;
	}

	sf::Vector2f GetMousePosInView(const sf::View& view)
	{
		return Window::GetWindow().mapPixelToCoords(sf::Mouse::getPosition(Window::GetWindow()), view);
	}

	sf::Vector2f GetMousePosInView(const sf::View& view, sf::Vector2i relativeMousePos)
	{
		return Window::GetWindow().mapPixelToCoords(relativeMousePos, view);
	}
}