#include "Window.h"
#include "Logger.h"
#include "InputManager.h"
#include "Menu.h"
#include "UI.h"
#include "EvolutionManager.h"
#include <vector>

namespace Window
{
	// Private
	sf::RenderWindow m_window;
	WindowStates m_state;
	std::vector<sf::Drawable*> m_lateDraws;
	sf::View m_defaultView;
	
	Event m_windowClosedEvent;
	Event<sf::Vector2u> m_windowResizedEvent;

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
				break;
			case sf::Event::Resized:
				m_windowResizedEvent.Call(m_window.getSize());
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

	// Public

	void Init()
	{
		static bool initComplete = false;

		if (!initComplete)
		{
			m_window.create(sf::VideoMode{ 800, 600 }, "Matthew Pawsey NEA Project");
			m_window.setFramerateLimit(MENU_FRAMERATE);
			m_window.setKeyRepeatEnabled(false);
			m_defaultView = m_window.getDefaultView();

			m_state = Menu;

			UI::Init();
			Menu::Init();
			Evolution::EvolutionManager::Init();

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

			while (m_window.isOpen())
			{
				PollWindowEvents();

				m_window.clear();

				switch (m_state)
				{
				case Menu:
					Menu::Update();
					break;
				case Evolution:
					Evolution::EvolutionManager::Update();
					break;
				}

				for (sf::Drawable* drawable : m_lateDraws)
					m_window.draw(*drawable);

				m_window.display();
			}

			m_windowClosedEvent.Call();
		}
		else
		{
			m_logger.Error("Run attempted after program started running.");
		}
	}

	void SetWindowState(WindowStates state)
	{
		m_state = state;
	}

	sf::RenderWindow& GetWindow()
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

	const sf::View& GetDefaultWindowView()
	{
		return m_defaultView;
	}

	const Event<>& GetWindowClosedEvent()
	{
		return m_windowClosedEvent;
	}

	const Event<sf::Vector2u>& GetWindowResizedEvent()
	{
		return m_windowResizedEvent;
	}

	void AddToLateDraw(sf::Drawable& drawable)
	{
		m_lateDraws.push_back(&drawable);
	}

	void RemoveFromLateDraw(sf::Drawable& drawable)
	{
		if (auto i = std::find(m_lateDraws.begin(), m_lateDraws.end(), &drawable); i != m_lateDraws.end())
			m_lateDraws.erase(i);
	}
}