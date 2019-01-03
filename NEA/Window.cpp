#include "Window.h"
#include "Logger.h"
#include "InputManager.h"
#include "UI.h"
#include "Button.h"
#include "TextField.h"

namespace Window
{
	// Private
	sf::RenderWindow m_window;
	WindowStates m_state;
	
	
	Event m_windowClosedEvent;

	Logger m_logger{ "Window" };


	void PollWindowEvents()
	{
		sf::Event e;
		while (m_window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				m_window.close();
				m_windowClosedEvent.Call();
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
				InputManager::MouseScrolled(e.mouseWheel.delta);
				break;
			case sf::Event::TextEntered:
				InputManager::TextEntered(e.text.unicode);
				break;
			}
		}
	}

	// Public
	enum WindowStates
	{
		Menu
	};


	void Init()
	{
		static bool initComplete = false;

		if (!initComplete)
		{
			m_window.create(sf::VideoMode{ 800, 600 }, "Matthew Pawsey NEA Project");
			m_window.setKeyRepeatEnabled(false);

			m_state = Menu;

			UI::Init();

			initComplete = true;
		}
		else
		{
			m_logger.Error("Init attempted after completion.");
		}
	}

	void Run()
	{
		static bool running = false;

		if (!running)
		{
			running = true;

			UI::Button b{ sf::Vector2f{0.f, 0.f}, m_window.getDefaultView(), "Text" };
			UI::TextField f{ sf::Vector2f{100.f, 100.f}, m_window.getDefaultView(), sf::Vector2f{500.f, 36.f}, UI::TextField::Integer };
			f.SetActive(true);

			while (m_window.isOpen())
			{
				PollWindowEvents();

				m_window.clear();


				m_window.draw(b);
				m_window.draw(f);

				m_window.display();
			}
		}
		else
		{
			m_logger.Error("Run attempted after program started running.");
		}
	}

	const sf::RenderWindow& GetWindow()
	{
		return m_window;
	}

	const WindowStates GetWindowState()
	{
		return m_state;
	}

	const sf::Vector2u GetWindowSize()
	{
		return m_window.getSize();
	}

	Event<>& GetWindowClosedEvent()
	{
		return m_windowClosedEvent;
	}
}