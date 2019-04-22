#pragma once

#include "Button.h"
#include "TextField.h"

namespace Menu
{
	class SettingsMenu
	{
	private:

		// View
		sf::View m_settingsView;

		// The different buttons and what goes with them

		// WIDTH
		const double m_widthMinVal = 15.0;
		const double m_widthMaxVal = 25.0;
		sf::Text m_widthText;
		UI::UIButton m_widthPlusBtn;
		UI::UIButton m_widthMinusBtn;
		UI::UITextField m_widthTF;

		// HEIGHT
		const double m_heightMinVal = 15.0;
		const double m_heightMaxVal = 25.0;
		sf::Text m_heightText;
		UI::UIButton m_heightPlusBtn;
		UI::UIButton m_heightMinusBtn;
		UI::UITextField m_heightTF;

		// RAY COUNT
		const int m_rayCountMinVal = 3;
		const int m_rayCountMaxVal = 7;
		sf::Text m_rayCountText;
		UI::UIButton m_rayCountPlusBtn;
		UI::UIButton m_rayCountMinusBtn;
		UI::UITextField m_rayCountTF;

		// RAY SIZE
		const double m_raySizeMinVal = 100.0;
		const double m_raySizeMaxVal = 250.0;
		sf::Text m_raySizeText;
		UI::UIButton m_raySizePlusBtn;
		UI::UIButton m_raySizeMinusBtn;
		UI::UITextField m_raySizeTF;

		// MUTATE SIZE
		const double m_mutSizeMinVal = 0.1;
		const double m_mutSizeMaxVal = 2.5;
		sf::Text m_mutSizeText;
		UI::UIButton m_mutSizePlusBtn;
		UI::UIButton m_mutSizeMinusBtn;
		UI::UITextField m_mutSizeTF;

		// ENGINE POWER
		const double m_enginePowMinVal = 5.0;
		const double m_enginePowMaxVal = 15.0;
		sf::Text m_enginePowText;
		UI::UIButton m_enginePowPlusBtn;
		UI::UIButton m_enginePowMinusBtn;
		UI::UITextField m_enginePowTF;

		// ROTATION POWER
		const double m_rotPowMinVal = 0.5;
		const double m_rotPowMaxVal = 2.5;
		sf::Text m_rotPowText;
		UI::UIButton m_rotPowPlusBtn;
		UI::UIButton m_rotPowMinusBtn;
		UI::UITextField m_rotPowTF;

		// MUTATE PERCENTAGE
		const double m_mutPCMinVal = 0.0;
		const double m_mutPCMaxVal = 100.0;
		sf::Text m_mutPCText;
		UI::UIButton m_mutPCPlusBtn;
		UI::UIButton m_mutPCMinusBtn;
		UI::UITextField m_mutPCTF;

		// SPLICE PERCENTAGE
		const double m_splicePCMinVal = 0.0;
		const double m_splicePCMaxVal = 100.0;
		sf::Text m_splicePCText;
		UI::UIButton m_splicePCPlusBtn;
		UI::UIButton m_splicePCMinusBtn;
		UI::UITextField m_splicePCTF;

		// SPLICE PERCENTAGE
		sf::Text m_seedText;
		UI::UITextField m_seedTF;

		// Other buttons
		UI::UIButton m_backBtn, m_nextBtn, m_advSettingsBtn;

	public:
		// Initialises the settings menu screen
		void Init();
		// Updates the settings menu screen (includes drawing to the window)
		void Update();
		// Loads the settings menu screen
		void Load();
		// Unloads the settings menu screen
		void Unload();
	};
}