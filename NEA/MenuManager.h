#pragma once

#include "MainMenu.h"
#include "SettingsMenu.h"
#include "MapSelectMenu.h"
#include "CarSelectMenu.h"
#include "AdvancedSettingsMenu.h"

namespace Menu
{
	// The possible states for the menu
	enum class MenuState
	{
		MainMenu,
		StartConfig,
		AdvancedConfig,
		StartCar,
		StartMap,
		None
	};

	class MenuManager
	{
	private:
		// The current menu state
		MenuState m_menuState;

		// The menu screens
		MainMenu m_mainMenu;
		SettingsMenu m_settingsMenu;
		MapSelectMenu m_mapSelectMenu;
		CarSelectMenu m_carSelectMenu;
		AdvancedSettingsMenu m_advSettingsMenu;

	public:
		// Returns the MenuManager used throughout the program
		static MenuManager& GetMenuManager();

		// Initialises the values for the menu manager
		void Init();

		// Updates the menu manager (includes drawing to window)
		void Update();

		// Changes the state of the menu
		void GoToState(MenuState newState);
	};
}