#pragma once

#include "Button.h"
#include "CheckBox.h"
#include "TextField.h"
#include <vector>

namespace Menu
{

	/*
	Deals with the neural network sizes
	and	reproduction stuff
	*/

	class AdvancedSettingsMenu
	{
	private:
		sf::View m_advSettingsView;

		// Single Reproduce Stuff
		UI::CheckBox m_onlyPairOnceCB;
		sf::Text m_onlyPairOnceText;

		// Save/kill count
		UI::TextField m_saveTF, m_killTF;

		// Stuff for custom NN
		std::vector<UI::TextField> m_layerSizes;
		UI::Button m_addLayer, m_removeLayer;

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}