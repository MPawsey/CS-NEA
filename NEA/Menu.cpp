#include "Menu.h"

#include "MainMenu.h"
#include "Event.h"

namespace Menu
{
	MenuState m_menuState;

	enum class MenuState
	{
		MainMenu,
		StartConfig,
		StartCar,
		StartMap
	};

	void Init()
	{
		m_menuState = MenuState::MainMenu;

		MainMenu::Init();
		MainMenu::Load();
	}

	void Update()
	{
		switch (m_menuState)
		{
		case MenuState::MainMenu:
			MainMenu::Update();
			break;
		}
	}

	void GoToState(MenuState newState)
	{
		// Unload previous state
		switch (m_menuState)
		{
		case MenuState::MainMenu:
			MainMenu::Unload();
			break;
		}

		// Load new state
		switch (m_menuState = newState)
		{
		case MenuState::MainMenu:
			MainMenu::Load();
			break;
		}
	}

}