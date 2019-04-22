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
		// Other
		sf::View m_advSettingsView;
		UI::UIButton m_backBtn;

		// Single Reproduce Stuff
		UI::UICheckBox m_onlyPairOnceCB;
		sf::Text m_onlyPairOnceText;

		// Population size
		const int m_popSizeMinVal = 15;
		const int m_popSizeMaxVal = 35;
		sf::Text m_popSizeText;
		UI::UIButton m_popSizePlusBtn;
		UI::UIButton m_popSizeMinusBtn;
		UI::UITextField m_popSizeTF;

		// Save count
		sf::Text m_saveText;
		UI::UIButton m_savePlusBtn;
		UI::UIButton m_saveMinusBtn;
		UI::UITextField m_saveTF;

		// Kill count
		sf::Text m_killText;
		UI::UIButton m_killPlusBtn;
		UI::UIButton m_killMinusBtn;
		UI::UITextField m_killTF;

		// Stuff for custom NN
		sf::Text m_layersText;
		sf::View m_layerView;
		sf::RectangleShape m_layersBackground;
		std::vector<UI::UITextField*> m_layerSizes;
		UI::UIButton m_addLayerBtn, m_removeLayer;
		UI::UISlider m_layerSlider;
		float m_layerSliderMax = -1;

		// Events
		void OnPairCBUpdate(bool isChecked); 
		void CheckTFValues(bool isSaveTF);
		void OnMouseScrolled(int delta);

	public:

		// Initialises the advanced settings menu
		void Init();
		
		// Updates the advanced settings menu (includes drawing to window)
		void Update();

		// Loads the advanced settings menu
		void Load();

		// Unloads the advanced settings menu
		void Unload();
	};
}