#include "Menu.h"

#include "MainMenu.h"
#include "SettingsMenu.h"
#include "Event.h"

namespace Menu
{
	MenuState m_menuState;

		void Init()
	{
		m_menuState = MenuState::MainMenu;

		MainMenu::Init();
		SettingsMenu::Init();

		MainMenu::Load();
	}

	void Update()
	{
		switch (m_menuState)
		{
		case MenuState::MainMenu:
			MainMenu::Update();
			break;
		case MenuState::StartConfig:
			SettingsMenu::Update();
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
		case MenuState::StartConfig:
			SettingsMenu::Unload();
			break;
		}

		// Load new state
		switch (m_menuState = newState)
		{
		case MenuState::MainMenu:
			MainMenu::Load();
			break;
		case MenuState::StartConfig:
			SettingsMenu::Load();
			break;
		}
	}

}