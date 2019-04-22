#pragma once

#include "Button.h"

namespace Menu
{
	class MainMenu
	{
	private:
		// View
		sf::View m_mainMenuView;

		// Buttons
		UI::UIButton m_startConfigBtn;
		UI::UIButton m_startSavedBtn;
		UI::UIButton m_mapEditorBtn;

	public:
		// Initialises the main menu screen
		void Init();
		// Updates the main menu screen (includes drawing to window)
		void Update();
		// Loads the main menu screen
		void Load();
		// Unloads the main menu screen
		void Unload();
	};
}