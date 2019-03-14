#pragma once

#include "Button.h"
#include "TextField.h"

namespace Menu
{
	class SettingsMenu
	{
	private:

		// Private
		sf::View m_settingsView;

		// WIDTH
		const double m_widthMinVal = 15.0;
		const double m_widthMaxVal = 25.0;
		sf::Text m_widthLabel;
		UI::Button m_widthPlusBtn;
		UI::Button m_widthMinusBtn;
		UI::TextField m_widthTF;

		// HEIGHT
		const double m_heightMinVal = 15.0;
		const double m_heightMaxVal = 25.0;
		sf::Text m_heightLabel;
		UI::Button m_heightPlusBtn;
		UI::Button m_heightMinusBtn;
		UI::TextField m_heightTF;

		// RAY COUNT
		const int m_rayCountMinVal = 3;
		const int m_rayCountMaxVal = 7;
		sf::Text m_rayCountLabel;
		UI::Button m_rayCountPlusBtn;
		UI::Button m_rayCountMinusBtn;
		UI::TextField m_rayCountTF;

		// RAY SIZE
		const double m_raySizeMinVal = 100.0;
		const double m_raySizeMaxVal = 250.0;
		sf::Text m_raySizeLabel;
		UI::Button m_raySizePlusBtn;
		UI::Button m_raySizeMinusBtn;
		UI::TextField m_raySizeTF;


		// POPULATION SIZE
		const int m_popSizeMinVal = 15;
		const int m_popSizeMaxVal = 35;
		sf::Text m_popSizeLabel;
		UI::Button m_popSizePlusBtn;
		UI::Button m_popSizeMinusBtn;
		UI::TextField m_popSizeTF;

		// ENGINE POWER
		const double m_enginePowMinVal = 5.0;
		const double m_enginePowMaxVal = 15.0;
		sf::Text m_enginePowLabel;
		UI::Button m_enginePowPlusBtn;
		UI::Button m_enginePowMinusBtn;
		UI::TextField m_enginePowTF;

		// ROTATION POWER
		const double m_rotPowMinVal = 0.5;
		const double m_rotPowMaxVal = 2.5;
		sf::Text m_rotPowLabel;
		UI::Button m_rotPowPlusBtn;
		UI::Button m_rotPowMinusBtn;
		UI::TextField m_rotPowTF;

		// MUTATE PERCENTAGE
		const double m_mutPCMinVal = 0.0;
		const double m_mutPCMaxVal = 100.0;
		sf::Text m_mutPCLabel;
		UI::Button m_mutPCPlusBtn;
		UI::Button m_mutPCMinusBtn;
		UI::TextField m_mutPCTF;

		// SPLICE PERCENTAGE
		const double m_splicePCMinVal = 0.0;
		const double m_splicePCMaxVal = 100.0;
		sf::Text m_splicePCLabel;
		UI::Button m_splicePCPlusBtn;
		UI::Button m_splicePCMinusBtn;
		UI::TextField m_splicePCTF;

		// SPLICE PERCENTAGE
		sf::Text m_seedLabel;
		UI::TextField m_seedTF;

		UI::Button m_backBtn, m_nextBtn, m_advSettingsBtn;

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}