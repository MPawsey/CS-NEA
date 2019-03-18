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

		// Population size
		const int m_popSizeMinVal = 15;
		const int m_popSizeMaxVal = 35;
		sf::Text m_popSizeLabel;
		UI::Button m_popSizePlusBtn;
		UI::Button m_popSizeMinusBtn;
		UI::TextField m_popSizeTF;

		// Save count
		sf::Text m_saveLabel;
		UI::Button m_savePlusBtn;
		UI::Button m_saveMinusBtn;
		UI::TextField m_saveTF;

		// Kill count
		sf::Text m_killLabel;
		UI::Button m_killPlusBtn;
		UI::Button m_killMinusBtn;
		UI::TextField m_killTF;

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