#include "MenuManager.h"

#include "Event.h"

namespace Menu
{
	MenuManager& MenuManager::GetMenuManager()
	{
		static MenuManager menuManager{};
		return menuManager;
	}

	void MenuManager::Init()
	{
		m_menuState = MenuState::MainMenu;

		m_mainMenu.Init();
		m_settingsMenu.Init();
		m_mapSelectMenu.Init();
		m_carSelectMenu.Init();
		m_advSettingsMenu.Init();

		m_mainMenu.Load();
	}

	void MenuManager::Update()
	{
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
		switch (m_menuState = newState)
		{
		case MenuState::MainMenu:
			m_mainMenu.Load();
			break;
		case MenuState::StartConfig:
			m_settingsMenu.Load();
			break;
		case MenuState::StartMap:
			m_mapSelectMenu.Load();
			break;
		case MenuState::StartCar:
			m_carSelectMenu.Load();
			break;
		case MenuState::AdvancedConfig:
			m_advSettingsMenu.Load();
			break;
		}
	}

}