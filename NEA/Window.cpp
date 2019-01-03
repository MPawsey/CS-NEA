#include "Window.h"
#include "Logger.h"

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

			m_state = Menu;

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




				m_window.display();
			}
		}
		else
		{
			m_logger.Error("Run attempted after program started running.");
		}
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