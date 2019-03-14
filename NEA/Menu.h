#pragma once

#include "MainMenu.h"
#include "SettingsMenu.h"
#include "MapSelectMenu.h"
#include "CarSelectMenu.h"
#include "AdvancedSettingsMenu.h"

namespace Menu
{
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

		MenuState m_menuState;
		MainMenu m_mainMenu;
		SettingsMenu m_settingsMenu;
		MapSelectMenu m_mapSelectMenu;
		CarSelectMenu m_carSelectMenu;
		AdvancedSettingsMenu m_advSettingsMenu;

	public:
		static MenuManager& GetMenuManager();

		void Init();
		void Update();

		void GoToState(MenuState newState);
	};
}