#pragma once

#include "Menu.h"
#include "Button.h"
#include "CheckBox.h"
#include "TextField.h"

namespace Menu
{
	class AdvancedSettingsMenu : public Menu
	{
	private:
		sf::View m_advSettingsView;

		// Single Reproduce Stuff
		UI::CheckBox m_onlyPairOnceCB;
		sf::Text m_onlyPairOnceText;

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}