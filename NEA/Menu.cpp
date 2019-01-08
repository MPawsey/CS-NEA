#include "Menu.h"

#include "MainMenu.h"
#include "SettingsMenu.h"
#include "MapSelectMenu.h"
#include "Event.h"

namespace Menu
{
	MenuState m_menuState;

	void Init()
	{
		m_menuState = MenuState::MainMenu;

		MainMenu::Init();
		SettingsMenu::Init();
		MapSelectMenu::Init();

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
		case MenuState::StartMap:
			MapSelectMenu::Update();
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
		case MenuState::StartMap:
			MapSelectMenu::Unload();
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
		case MenuState::StartMap:
			MapSelectMenu::Load();
			break;
		}
	}

}