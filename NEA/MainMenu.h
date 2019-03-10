#pragma once

#include "Menu.h"
#include "Button.h"

namespace Menu
{
	class MainMenu : public Menu
	{
	private:
		// Private
		sf::View m_mainMenuView;

		UI::Button m_startConfigBtn;
		UI::Button m_startSavedBtn;
		UI::Button m_mapEditorBtn;

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}