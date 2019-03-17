#pragma once

#include "Button.h"
#include "CheckBox.h"
#include "TextField.h"
#include "Slider.h"
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

		UI::Button m_backBtn;

		// Single Reproduce Stuff
		UI::CheckBox m_onlyPairOnceCB;
		sf::Text m_onlyPairOnceText;

		// Save/kill count
		UI::TextField m_saveTF, m_killTF;

		// Stuff for custom NN
		sf::Text m_layersText;
		sf::View m_layerView;
		sf::RectangleShape m_layersBackground;
		std::vector<UI::TextField*> m_layerSizes;
		UI::Button m_addLayer, m_removeLayer;
		UI::Slider m_layerSlider;
		float m_layerSliderMax = -1;

		void OnPairCBUpdate(bool isChecked); 
		void CheckTFValues(bool isSaveTF);
		void OnMouseScrolled(int delta);

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}