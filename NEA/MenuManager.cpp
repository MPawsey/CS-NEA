#include "MenuManager.h"

#include "Event.h"

namespace Menu
{
	// Returns a static instance of MenuManager to be used by the program
	MenuManager& MenuManager::GetMenuManager()
	{
		// Gets the static instance
		static MenuManager menuManager{};
		return menuManager;
	}

	// Initialises the menu manager
	void MenuManager::Init()
	{
		// Sets the starting state of the menu
		m_menuState = MenuState::MainMenu;

		// Initialises each of the menu screens
		m_mainMenu.Init();
		m_settingsMenu.Init();
		m_mapSelectMenu.Init();
		m_carSelectMenu.Init();
		m_advSettingsMenu.Init();

		// Loads the main menu screen
		m_mainMenu.Load();
	}

	// Updates the current menu screen
	void MenuManager::Update()
	{
		// Determines the current menu and updates it
		switch (m_menuState)
		{
		case MenuState::MainMenu:
			m_mainMenu.Update();
			break;
		case MenuState::StartConfig:
			m_settingsMenu.Update();
			break;
		case MenuState::StartMap:
			m_mapSelectMenu.Update();
			break;
		case MenuState::StartCar:
			m_carSelectMenu.Update();
			break;
		case MenuState::AdvancedConfig:
			m_advSettingsMenu.Update();
			break;
		}
	}

	// Changes the state of the menu
	void MenuManager::GoToState(MenuState newState)
	{
		// Unload previous state
		switch (m_menuState)
		{
		case MenuState::MainMenu:
			m_mainMenu.Unload();
			break;
		case MenuState::StartConfig:
			m_settingsMenu.Unload();
			break;
		case MenuState::StartMap:
			m_mapSelectMenu.Unload();
			break;
		case MenuState::StartCar:
			m_carSelectMenu.Unload();
			break;
		case MenuState::AdvancedConfig:
			m_advSettingsMenu.Unload();
			break;
		}

		// Load new state
		switch (newState)
		{
		case MenuState::MainMenu:
			m_mainMenu.Load();
			break;
		case MenuState::StartConfig:
			m_settingsMenu.Load();
			break;
		case MenuState::StartMap:
			m_mapSelectMenu.Load(m_menuState);
			break;
		case MenuState::StartCar:
			m_carSelectMenu.Load();
			break;
		case MenuState::AdvancedConfig:
			m_advSettingsMenu.Load();
			break;
		}

		// Updates the menu state to the new state
		m_menuState = newState;
	}

}