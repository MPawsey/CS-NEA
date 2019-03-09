#include "Menu.h"

#include "MainMenu.h"
#include "SettingsMenu.h"
#include "MapSelectMenu.h"
#include "CarSelectMenu.h"
#include "AdvancedSettingsMenu.h"
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
		CarSelectMenu::Init();
		AdvancedSettingsMenu::Init();

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
		case MenuState::StartCar:
			CarSelectMenu::Update();
			break;
		case MenuState::AdvancedConfig:
			AdvancedSettingsMenu::Update();
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
		case MenuState::StartCar:
			CarSelectMenu::Unload();
			break;
		case MenuState::AdvancedConfig:
			AdvancedSettingsMenu::Unload();
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
		case MenuState::StartCar:
			CarSelectMenu::Load();
			break;
		case MenuState::AdvancedConfig:
			AdvancedSettingsMenu::Load();
			break;
		}
	}

}