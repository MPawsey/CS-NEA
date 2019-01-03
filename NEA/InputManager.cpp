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

	sf::String textInput;
	Event<sf::String> textEnteredEvent;

	// Public

	void KeyPressed(sf::Keyboard::Key key)
	{
		if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
		{
			if (key == sf::Keyboard::BackSpace)
			{
				// Calls update when text is removed
				textInput.erase(textInput.getSize() - 1);
				textEnteredEvent.Call(textInput);
			}

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

	void TextEntered(sf::Uint32 character)
	{
		textInput.insert(textInput.getSize(), character);
		textEnteredEvent.Call(textInput);
	}

	const Event<>& GetKeyPressedEvent(sf::Keyboard::Key key)
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

	const Event<>& GetKeyReleasedEvent(sf::Keyboard::Key key)
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

	const Event<sf::Keyboard::Key>& GetGenericKeyPressedEvent()
	{
		return genericKeyPressedEvent;
	}

	const Event<sf::Keyboard::Key>& GetGenericKeyReleasedEvent()
	{
		return genericKeyReleasedEvent;
	}


	const Event<>& GetMousePressedEvent(sf::Mouse::Button btn)
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

	const Event<>& GetMouseReleasedEvent(sf::Mouse::Button btn)
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

	const Event<sf::Mouse::Button>& GetGenericMousePressedEvent()
	{
		return genericMousePressedEvent;
	}

	const Event<sf::Mouse::Button>& GetGenericMouseReleasedEvent()
	{
		return genericMouseReleasedEvent;
	}

	const Event<int>& GetMouseScrolledEvent()
	{
		return mouseScrolledEvents;
	}

	const Event<sf::String>& GetTextEnteredEvent()
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

	bool IsMouseInView(const sf::View& view, sf::Vector2f& viewPos)
	{
		const sf::RenderWindow& window = Window::GetWindow();

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = (sf::Vector2f)mousePos;
		sf::Vector2u windowSize = Window::GetWindowSize();
		pos.x /= windowSize.x;
		pos.y /= windowSize.y;
		
		if (view.getViewport().contains(pos))
		{
			viewPos = window.mapPixelToCoords(mousePos);
			return true;
		}
		return false;
	}

	unsigned int GetTextEnteredSize()
	{
		return textInput.getSize();
	}
}