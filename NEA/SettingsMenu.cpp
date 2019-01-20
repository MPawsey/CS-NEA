#include "SettingsMenu.h"
#include "UI.h"
#include "Window.h"
#include <algorithm>
#include "Menu.h"
#include "EvolutionManager.h"

namespace Menu::SettingsMenu
{
	// Private
	sf::View m_settingsView;

	// WIDTH
	constexpr double m_widthMinVal = 15.0;
	constexpr double m_widthMaxVal = 25.0;
	sf::Text m_widthLabel;
	UI::Button m_widthPlusBtn;
	UI::Button m_widthMinusBtn;
	UI::TextField m_widthTF;

	// HEIGHT
	constexpr double m_heightMinVal = 15.0;
	constexpr double m_heightMaxVal = 25.0;
	sf::Text m_heightLabel;
	UI::Button m_heightPlusBtn;
	UI::Button m_heightMinusBtn;
	UI::TextField m_heightTF;

	// RAY COUNT
	constexpr int m_rayCountMinVal = 3;
	constexpr int m_rayCountMaxVal = 7;
	sf::Text m_rayCountLabel;
	UI::Button m_rayCountPlusBtn;
	UI::Button m_rayCountMinusBtn;
	UI::TextField m_rayCountTF;

	// RAY SIZE
	constexpr double m_raySizeMinVal = 100.0;
	constexpr double m_raySizeMaxVal = 250.0;
	sf::Text m_raySizeLabel;
	UI::Button m_raySizePlusBtn;
	UI::Button m_raySizeMinusBtn;
	UI::TextField m_raySizeTF;


	// POPULATION SIZE
	constexpr int m_popSizeMinVal = 15;
	constexpr int m_popSizeMaxVal = 25;
	sf::Text m_popSizeLabel;
	UI::Button m_popSizePlusBtn;
	UI::Button m_popSizeMinusBtn;
	UI::TextField m_popSizeTF;

	// ENGINE POWER
	constexpr double m_enginePowMinVal = 5.0;
	constexpr double m_enginePowMaxVal = 15.0;
	sf::Text m_enginePowLabel;
	UI::Button m_enginePowPlusBtn;
	UI::Button m_enginePowMinusBtn;
	UI::TextField m_enginePowTF;

	// ROTATION POWER
	constexpr double m_rotPowMinVal = 0.5;
	constexpr double m_rotPowMaxVal = 2.5;
	sf::Text m_rotPowLabel;
	UI::Button m_rotPowPlusBtn;
	UI::Button m_rotPowMinusBtn;
	UI::TextField m_rotPowTF;

	// MUTATE PERCENTAGE
	constexpr double m_mutPCMinVal = 0.0;
	constexpr double m_mutPCMaxVal = 1.0;
	sf::Text m_mutPCLabel;
	UI::Button m_mutPCPlusBtn;
	UI::Button m_mutPCMinusBtn;
	UI::TextField m_mutPCTF;

	// SPLICE PERCENTAGE
	constexpr double m_splicePCMinVal = 0.0;
	constexpr double m_splicePCMaxVal = 1.0;
	sf::Text m_splicePCLabel;
	UI::Button m_splicePCPlusBtn;
	UI::Button m_splicePCMinusBtn;
	UI::TextField m_splicePCTF;

	// SPLICE PERCENTAGE
	sf::Text m_seedLabel;
	UI::TextField m_seedTF;

	UI::Button m_backBtn, m_nextBtn;

	std::string DoubleToString(double d)
	{
		std::string s = std::to_string(d);
		return s.erase(s.find_last_not_of('0') + 1, std::string::npos).back() == '.' ? (s.pop_back(), s) : s;
	}

	void ClampTextFieldDoubleValue(UI::TextField& text, double boundLower, double boundUpper)
	{
		if (text.GetRawText().getSize() == 1)
			text.SetRawText("0");
		text.SetRawText(DoubleToString(std::clamp(std::stod(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	void ClampTextFieldIntegerValue(UI::TextField& text, int boundLower, int boundUpper)
	{
		if (text.GetRawText().getSize() == 1)
			text.SetRawText("0");
		text.SetRawText(DoubleToString(std::clamp(std::stoi(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	// Public

	void Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_settingsView = window.getDefaultView();
		const sf::Font& font = UI::GetFont();

		float xPos1 = 50.f;
		float xPos2 = 300.f;
		float xPos3 = 550.f;
		float yPos = 50.f;
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yGap = 125.f;
		float xGap = 10.f;
		float textFieldWidth = 100.f;
		float buttonWidth = 35.f;
		float topPadding = 2.f, hzPadding = 4.f;


		/* LAYOUT
		 * 
		 * WIDTH	RAYCOUNT	ENGINE POW
		 * HEIGHT	RAY SIZE	ROTATION POW
		 * POP SIZE	MUTATE PC	SPLICE PC
		 *			SEED
		 * 
		 */

		// WIDTH
		m_widthLabel = sf::Text{ "Car width", font };
		m_widthLabel.setPosition(xPos1, yPos);
		m_widthPlusBtn = UI::Button{ "+", m_settingsView };
		m_widthPlusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_widthPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthPlusBtn.SetCentreText(true);
		m_widthPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) + 0.5, m_widthMinVal, m_widthMaxVal))); });
		m_widthTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_widthTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_widthTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_widthTF, m_widthMinVal, m_widthMaxVal); });
		m_widthTF.SetTooltipText("[15.0-25.0] Default=15\nWidth of the car.");
		m_widthMinusBtn = UI::Button{ "-", m_settingsView };
		m_widthMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_widthMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthMinusBtn.SetCentreText(true);
		m_widthMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) - 0.5, m_widthMinVal, m_widthMaxVal))); });

		// RAY COUNT
		m_rayCountLabel = sf::Text{ "Ray count", font };
		m_rayCountLabel.setPosition(xPos2, yPos);
		m_rayCountPlusBtn = UI::Button{ "+", m_settingsView };
		m_rayCountPlusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_rayCountPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountPlusBtn.SetCentreText(true);
		m_rayCountPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) + 1, m_rayCountMinVal, m_rayCountMaxVal))); });
		m_rayCountTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_settingsView };
		m_rayCountTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_rayCountTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldIntegerValue(m_rayCountTF, m_rayCountMinVal, m_rayCountMaxVal); });
		m_rayCountTF.SetTooltipText("[3-7] Default=5\nThe number of detector rays\nthat the car uses to detect walls.");
		m_rayCountMinusBtn = UI::Button{ "-", m_settingsView };
		m_rayCountMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_rayCountMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountMinusBtn.SetCentreText(true);
		m_rayCountMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) - 1, m_rayCountMinVal, m_rayCountMaxVal))); });

		// ENGINE POWER
		m_enginePowLabel = sf::Text{ "Engine power", font };
		m_enginePowLabel.setPosition(xPos3, yPos);
		m_enginePowPlusBtn = UI::Button{ "+", m_settingsView };
		m_enginePowPlusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_enginePowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowPlusBtn.SetCentreText(true);
		m_enginePowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) + 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });
		m_enginePowTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_enginePowTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_enginePowTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldIntegerValue(m_enginePowTF, m_enginePowMinVal, m_enginePowMaxVal); });
		m_enginePowTF.SetTooltipText("[5.0-15.0] Default=10.0\nThe power of the engine.\ni.e the max speed of the car.");
		m_enginePowMinusBtn = UI::Button{ "-", m_settingsView };
		m_enginePowMinusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_enginePowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowMinusBtn.SetCentreText(true);
		m_enginePowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) - 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });

		yPos += yGap;

		// HEIGHT
		m_heightLabel = sf::Text{ "Car height", font };
		m_heightLabel.setPosition(xPos1, yPos);
		m_heightPlusBtn = UI::Button{ "+", m_settingsView };
		m_heightPlusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_heightPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightPlusBtn.SetCentreText(true);
		m_heightPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) + 0.5, m_heightMinVal, m_heightMaxVal))); });
		m_heightTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_heightTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_heightTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_heightTF, m_heightMinVal, m_heightMaxVal); });
		m_heightTF.SetTooltipText("[15.0-25.0] Default=25\nHeight of the car.");
		m_heightMinusBtn = UI::Button{ "-", m_settingsView };
		m_heightMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_heightMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightMinusBtn.SetCentreText(true);
		m_heightMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) - 0.5, m_heightMinVal, m_heightMaxVal))); });

		// RAY SIZE
		m_raySizeLabel = sf::Text{ "Ray size", font };
		m_raySizeLabel.setPosition(xPos2, yPos);
		m_raySizePlusBtn = UI::Button{ "+", m_settingsView };
		m_raySizePlusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_raySizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_raySizePlusBtn.SetCentreText(true);
		m_raySizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_raySizeTF.SetRawText(DoubleToString(std::clamp(std::stod(m_raySizeTF.GetRawText().toAnsiString()) + 25, m_raySizeMinVal, m_raySizeMaxVal))); });
		m_raySizeTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_raySizeTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_raySizeTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldIntegerValue(m_raySizeTF, m_raySizeMinVal, m_raySizeMaxVal); });
		m_raySizeTF.SetTooltipText("[100.0-250.0] Default=150\nThe size of the detector rays.");
		m_raySizeMinusBtn = UI::Button{ "-", m_settingsView };
		m_raySizeMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_raySizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_raySizeMinusBtn.SetCentreText(true);
		m_raySizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_raySizeTF.SetRawText(DoubleToString(std::clamp(std::stod(m_raySizeTF.GetRawText().toAnsiString()) - 25, m_raySizeMinVal, m_raySizeMaxVal))); });

		// ROTATION POWER
		m_rotPowLabel = sf::Text{ "Rotation power", font };
		m_rotPowLabel.setPosition(xPos3, yPos);
		m_rotPowPlusBtn = UI::Button{ "+", m_settingsView };
		m_rotPowPlusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_rotPowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowPlusBtn.SetCentreText(true);
		m_rotPowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) + 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });
		m_rotPowTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_rotPowTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_rotPowTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_rotPowTF, m_rotPowMinVal, m_rotPowMaxVal); });
		m_rotPowTF.SetTooltipText("[0.5-2.5] Default=1.0\nThe rotational power of the car.\ni.e the max rotational speed.");
		m_rotPowMinusBtn = UI::Button{ "-", m_settingsView };
		m_rotPowMinusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_rotPowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowMinusBtn.SetCentreText(true);
		m_rotPowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) - 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });

		yPos += yGap;

		// POPULATION SIZE
		m_popSizeLabel = sf::Text{ "Population size", font };
		m_popSizeLabel.setPosition(xPos1, yPos);
		m_popSizePlusBtn = UI::Button{ "+", m_settingsView };
		m_popSizePlusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_popSizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizePlusBtn.SetCentreText(true);
		m_popSizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) + 1, m_popSizeMinVal, m_popSizeMaxVal))); });
		m_popSizeTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_settingsView };
		m_popSizeTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_popSizeTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_popSizeTF, m_popSizeMinVal, m_popSizeMaxVal); });
		m_popSizeTF.SetTooltipText("[15-25] Default=25\nThe number of cars in a generation.");
		m_popSizeMinusBtn = UI::Button{ "-", m_settingsView };
		m_popSizeMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_popSizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizeMinusBtn.SetCentreText(true);
		m_popSizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) - 1, m_popSizeMinVal, m_popSizeMaxVal))); });

		// MUTATE PC
		m_mutPCLabel = sf::Text{ "Mutate %", font };
		m_mutPCLabel.setPosition(xPos2, yPos);
		m_mutPCPlusBtn = UI::Button{ "+", m_settingsView };
		m_mutPCPlusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_mutPCPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutPCPlusBtn.SetCentreText(true);
		m_mutPCPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutPCTF.SetRawText(DoubleToString(std::clamp(std::stod(m_mutPCTF.GetRawText().toAnsiString()) + 0.05, m_mutPCMinVal, m_mutPCMaxVal))); });
		m_mutPCTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_mutPCTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_mutPCTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_mutPCTF, m_mutPCMinVal, m_mutPCMaxVal); });
		m_mutPCTF.SetTooltipText("[0.0-1.0] Default=0.25\nThe percentage a weight in the\nneural network will mutate.");
		m_mutPCMinusBtn = UI::Button{ "-", m_settingsView };
		m_mutPCMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_mutPCMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutPCMinusBtn.SetCentreText(true);
		m_mutPCMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutPCTF.SetRawText(DoubleToString(std::clamp(std::stod(m_mutPCTF.GetRawText().toAnsiString()) - 0.05, m_mutPCMinVal, m_mutPCMaxVal))); });

		// SPLICE PC
		m_splicePCLabel = sf::Text{ "Splice %", font };
		m_splicePCLabel.setPosition(xPos3, yPos);
		m_splicePCPlusBtn = UI::Button{ "+", m_settingsView };
		m_splicePCPlusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_splicePCPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_splicePCPlusBtn.SetCentreText(true);
		m_splicePCPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_splicePCTF.SetRawText(DoubleToString(std::clamp(std::stod(m_splicePCTF.GetRawText().toAnsiString()) + 0.05, m_splicePCMinVal, m_splicePCMaxVal))); });
		m_splicePCTF = UI::TextField{ textFieldWidth, UI::TextField::Decimal, m_settingsView };
		m_splicePCTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_splicePCTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_splicePCTF, m_splicePCMinVal, m_splicePCMaxVal); });
		m_splicePCTF.SetTooltipText("[0.0-1.0] Default=0.6\nThe percentage the weights\n between two cars swap\n when reproducing.");
		m_splicePCMinusBtn = UI::Button{ "-", m_settingsView };
		m_splicePCMinusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_splicePCMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_splicePCMinusBtn.SetCentreText(true);
		m_splicePCMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_splicePCTF.SetRawText(DoubleToString(std::clamp(std::stod(m_splicePCTF.GetRawText().toAnsiString()) - 0.05, m_splicePCMinVal, m_splicePCMaxVal))); });

		yPos += yGap;

		// SEED
		m_seedLabel = sf::Text{ "Seed", font };
		m_seedLabel.setPosition(xPos2 + buttonWidth + xGap, yPos);
		m_seedTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_settingsView };
		m_seedTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_seedTF.SetTooltipText("The seed to use for the\nrandom number generator in the\nsimulation. Leave blank if a random\nseed is wanted.");


		m_backBtn = UI::Button{ "Back", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(xPos1, window.getSize().y - yLineSpace);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { GoToState(MenuState::MainMenu); });

		m_nextBtn = UI::Button{ "Next", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_nextBtn.setPosition(675.f, window.getSize().y - yLineSpace);
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([&]() { GoToState(MenuState::StartMap); });


		// DEFAULT VALUES
		m_widthTF.SetRawText("15");
		m_heightTF.SetRawText("25");
		m_rayCountTF.SetRawText("5");
		m_raySizeTF.SetRawText("150");
		m_popSizeTF.SetRawText("25");
		m_enginePowTF.SetRawText("10");
		m_rotPowTF.SetRawText("1");
		m_mutPCTF.SetRawText("0.25");
		m_splicePCTF.SetRawText("0.6");
		m_seedTF.SetRawText("");

	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();


		window.draw(m_widthLabel);
		window.draw(m_widthPlusBtn);
		window.draw(m_widthTF);
		window.draw(m_widthMinusBtn);

		window.draw(m_heightLabel);
		window.draw(m_heightPlusBtn);
		window.draw(m_heightTF);
		window.draw(m_heightMinusBtn);

		window.draw(m_rayCountLabel);
		window.draw(m_rayCountPlusBtn);
		window.draw(m_rayCountTF);
		window.draw(m_rayCountMinusBtn);

		window.draw(m_raySizeLabel);
		window.draw(m_raySizePlusBtn);
		window.draw(m_raySizeTF);
		window.draw(m_raySizeMinusBtn);

		window.draw(m_popSizeLabel);
		window.draw(m_popSizePlusBtn);
		window.draw(m_popSizeTF);
		window.draw(m_popSizeMinusBtn);

		window.draw(m_enginePowLabel);
		window.draw(m_enginePowPlusBtn);
		window.draw(m_enginePowTF);
		window.draw(m_enginePowMinusBtn);

		window.draw(m_rotPowLabel);
		window.draw(m_rotPowPlusBtn);
		window.draw(m_rotPowTF);
		window.draw(m_rotPowMinusBtn);

		window.draw(m_mutPCLabel);
		window.draw(m_mutPCPlusBtn);
		window.draw(m_mutPCTF);
		window.draw(m_mutPCMinusBtn);

		window.draw(m_splicePCLabel);
		window.draw(m_splicePCPlusBtn);
		window.draw(m_splicePCTF);
		window.draw(m_splicePCMinusBtn);

		window.draw(m_seedLabel);
		window.draw(m_seedTF);
		
		window.draw(m_backBtn);
		window.draw(m_nextBtn);
	}

	void Load()
	{
		Window::GetWindow().setView(m_settingsView);

		m_widthPlusBtn.SetActive(true);
		m_widthTF.SetActive(true);
		m_widthMinusBtn.SetActive(true);

		m_heightPlusBtn.SetActive(true);
		m_heightTF.SetActive(true);
		m_heightMinusBtn.SetActive(true);

		m_rayCountPlusBtn.SetActive(true);
		m_rayCountTF.SetActive(true);
		m_rayCountMinusBtn.SetActive(true);

		m_raySizePlusBtn.SetActive(true);
		m_raySizeTF.SetActive(true);
		m_raySizeMinusBtn.SetActive(true);

		m_popSizePlusBtn.SetActive(true);
		m_popSizeTF.SetActive(true);
		m_popSizeMinusBtn.SetActive(true);

		m_enginePowPlusBtn.SetActive(true);
		m_enginePowTF.SetActive(true);
		m_enginePowMinusBtn.SetActive(true);

		m_rotPowPlusBtn.SetActive(true);
		m_rotPowTF.SetActive(true);
		m_rotPowMinusBtn.SetActive(true);

		m_mutPCPlusBtn.SetActive(true);
		m_mutPCTF.SetActive(true);
		m_mutPCMinusBtn.SetActive(true);

		m_splicePCPlusBtn.SetActive(true);
		m_splicePCTF.SetActive(true);
		m_splicePCMinusBtn.SetActive(true);

		m_seedTF.SetActive(true);

		m_backBtn.SetActive(true);
		m_nextBtn.SetActive(true);
	}

	void Unload()
	{
		EvolutionManager::CreateGenerationFromSettings(m_widthTF.GetFloatValue(), m_heightTF.GetFloatValue(), m_rayCountTF.GetUIntegerValue(), m_raySizeTF.GetFloatValue(),
			m_popSizeTF.GetUIntegerValue(), m_enginePowTF.GetFloatValue(), m_rotPowTF.GetFloatValue(), m_mutPCTF.GetDoubleValue(), m_splicePCTF.GetDoubleValue(), m_seedTF.GetRawText() == "" ? std::random_device{}() : (unsigned int)m_seedTF.GetIntegerValue());

		m_widthPlusBtn.SetActive(false);
		m_widthTF.SetActive(false);
		m_widthMinusBtn.SetActive(false);

		m_heightPlusBtn.SetActive(false);
		m_heightTF.SetActive(false);
		m_heightMinusBtn.SetActive(false);

		m_rayCountPlusBtn.SetActive(false);
		m_rayCountTF.SetActive(false);
		m_rayCountMinusBtn.SetActive(false);
		
		m_raySizePlusBtn.SetActive(false);
		m_raySizeTF.SetActive(false);
		m_raySizeMinusBtn.SetActive(false);

		m_popSizePlusBtn.SetActive(false);
		m_popSizeTF.SetActive(false);
		m_popSizeMinusBtn.SetActive(false);

		m_enginePowPlusBtn.SetActive(false);
		m_enginePowTF.SetActive(false);
		m_enginePowMinusBtn.SetActive(false);

		m_rotPowPlusBtn.SetActive(false);
		m_rotPowTF.SetActive(false);
		m_rotPowMinusBtn.SetActive(false);

		m_mutPCPlusBtn.SetActive(false);
		m_mutPCTF.SetActive(false);
		m_mutPCMinusBtn.SetActive(false);

		m_splicePCPlusBtn.SetActive(false);
		m_splicePCTF.SetActive(false);
		m_splicePCMinusBtn.SetActive(false);

		m_seedTF.SetActive(false);

		m_backBtn.SetActive(false);
		m_nextBtn.SetActive(false);
	}
}