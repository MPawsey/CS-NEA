#include "SettingsMenu.h"
#include "UI.h"
#include "Window.h"
#include <algorithm>
#include "Menu.h"

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
		float xPos2 = 500.f;
		float yPos = 50.f;
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yGap = 125.f;
		float xGap = 10.f;
		float textFieldWidth = 100.f;
		float buttonWidth = 35.f;

		// WIDTH
		m_widthLabel = sf::Text{ "Car width", font };
		m_widthLabel.setPosition(xPos1, yPos);
		m_widthPlusBtn = UI::Button{ "+", m_settingsView };
		m_widthPlusBtn.setPosition(xPos1, yPos + yLineSpace);
		m_widthPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthPlusBtn.SetCentreText(true);
		m_widthPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) + 0.5, m_widthMinVal, m_widthMaxVal))); });
		m_widthTF = UI::TextField{ sf::Vector2f{ xPos1 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_widthTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_widthTF, m_widthMinVal, m_widthMaxVal); });
		m_widthMinusBtn = UI::Button{ "-", m_settingsView };
		m_widthMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_widthMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthMinusBtn.SetCentreText(true);
		m_widthMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) - 0.5, m_widthMinVal, m_widthMaxVal))); });

		// HEIGHT
		m_heightLabel = sf::Text{ "Car height", font };
		m_heightLabel.setPosition(xPos2, yPos);
		m_heightPlusBtn = UI::Button{ "+", m_settingsView };
		m_heightPlusBtn.setPosition(xPos2, yPos + yLineSpace);
		m_heightPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightPlusBtn.SetCentreText(true);
		m_heightPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) + 0.5, m_heightMinVal, m_heightMaxVal))); });
		m_heightTF = UI::TextField{ sf::Vector2f{ xPos2 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_heightTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_heightTF, m_heightMinVal, m_heightMaxVal); });
		m_heightMinusBtn = UI::Button{ "-", m_settingsView };
		m_heightMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_heightMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightMinusBtn.SetCentreText(true);
		m_heightMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) - 0.5, m_heightMinVal, m_heightMaxVal))); });

		yPos += yGap;

		// RAY COUNT
		m_rayCountLabel = sf::Text{ "Ray count", font };
		m_rayCountLabel.setPosition(xPos1, yPos);
		m_rayCountPlusBtn = UI::Button{ "+", m_settingsView };
		m_rayCountPlusBtn.setPosition(xPos1, yPos + yLineSpace);
		m_rayCountPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountPlusBtn.SetCentreText(true);
		m_rayCountPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) + 1, m_rayCountMinVal, m_rayCountMaxVal))); });
		m_rayCountTF = UI::TextField{ sf::Vector2f{ xPos1 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_rayCountTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldIntegerValue(m_rayCountTF, m_rayCountMinVal, m_rayCountMaxVal); });
		m_rayCountMinusBtn = UI::Button{ "-", m_settingsView };
		m_rayCountMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_rayCountMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountMinusBtn.SetCentreText(true);
		m_rayCountMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) - 1, m_rayCountMinVal, m_rayCountMaxVal))); });



		// POPULATION SIZE
		m_popSizeLabel = sf::Text{ "Population size", font };
		m_popSizeLabel.setPosition(xPos2, yPos);
		m_popSizePlusBtn = UI::Button{ "+", m_settingsView };
		m_popSizePlusBtn.setPosition(xPos2, yPos + yLineSpace);
		m_popSizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizePlusBtn.SetCentreText(true);
		m_popSizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) + 1, m_popSizeMinVal, m_popSizeMaxVal))); });
		m_popSizeTF = UI::TextField{ sf::Vector2f{ xPos2 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_popSizeTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_popSizeTF, m_popSizeMinVal, m_popSizeMaxVal); });
		m_popSizeMinusBtn = UI::Button{ "-", m_settingsView };
		m_popSizeMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_popSizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizeMinusBtn.SetCentreText(true);
		m_popSizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) - 1, m_popSizeMinVal, m_popSizeMaxVal))); });

		yPos += yGap;

		// ENGINE POWER
		m_enginePowLabel = sf::Text{ "Engine power", font };
		m_enginePowLabel.setPosition(xPos1, yPos);
		m_enginePowPlusBtn = UI::Button{ "+", m_settingsView };
		m_enginePowPlusBtn.setPosition(xPos1, yPos + yLineSpace);
		m_enginePowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowPlusBtn.SetCentreText(true);
		m_enginePowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) + 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });
		m_enginePowTF = UI::TextField{ sf::Vector2f{ xPos1 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_enginePowTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldIntegerValue(m_enginePowTF, m_enginePowMinVal, m_enginePowMaxVal); });
		m_enginePowMinusBtn = UI::Button{ "-", m_settingsView };
		m_enginePowMinusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_enginePowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowMinusBtn.SetCentreText(true);
		m_enginePowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) - 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });

		// ROTATION POWER
		m_rotPowLabel = sf::Text{ "Rotation power", font };
		m_rotPowLabel.setPosition(xPos2, yPos);
		m_rotPowPlusBtn = UI::Button{ "+", m_settingsView };
		m_rotPowPlusBtn.setPosition(xPos2, yPos + yLineSpace);
		m_rotPowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowPlusBtn.SetCentreText(true);
		m_rotPowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) + 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });
		m_rotPowTF = UI::TextField{ sf::Vector2f{ xPos2 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_rotPowTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldDoubleValue(m_rotPowTF, m_rotPowMinVal, m_rotPowMaxVal); });
		m_rotPowMinusBtn = UI::Button{ "-", m_settingsView };
		m_rotPowMinusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace);
		m_rotPowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowMinusBtn.SetCentreText(true);
		m_rotPowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) - 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });

		m_backBtn = UI::Button{ "Back", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(xPos1, window.getSize().y - yLineSpace);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { GoToState(MenuState::MainMenu); });

		m_nextBtn = UI::Button{ "Next", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_nextBtn.setPosition(675.f, window.getSize().y - yLineSpace);
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([&]() { GoToState(MenuState::StartMap); });

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

		m_popSizePlusBtn.SetActive(true);
		m_popSizeTF.SetActive(true);
		m_popSizeMinusBtn.SetActive(true);

		m_enginePowPlusBtn.SetActive(true);
		m_enginePowTF.SetActive(true);
		m_enginePowMinusBtn.SetActive(true);

		m_rotPowPlusBtn.SetActive(true);
		m_rotPowTF.SetActive(true);
		m_rotPowMinusBtn.SetActive(true);

		m_backBtn.SetActive(true);
		m_nextBtn.SetActive(true);

		m_widthTF.SetRawText("15");
		m_heightTF.SetRawText("25");
		m_rayCountTF.SetRawText("5");
		m_popSizeTF.SetRawText("25");
		m_enginePowTF.SetRawText("10");
		m_rotPowTF.SetRawText("1");
	}

	void Unload()
	{
		m_widthPlusBtn.SetActive(false);
		m_widthTF.SetActive(false);
		m_widthMinusBtn.SetActive(false);

		m_heightPlusBtn.SetActive(false);
		m_heightTF.SetActive(false);
		m_heightMinusBtn.SetActive(false);

		m_rayCountPlusBtn.SetActive(false);
		m_rayCountTF.SetActive(false);
		m_rayCountMinusBtn.SetActive(false);

		m_popSizePlusBtn.SetActive(false);
		m_popSizeTF.SetActive(false);
		m_popSizeMinusBtn.SetActive(false);

		m_enginePowPlusBtn.SetActive(false);
		m_enginePowTF.SetActive(false);
		m_enginePowMinusBtn.SetActive(false);

		m_rotPowPlusBtn.SetActive(false);
		m_rotPowTF.SetActive(false);
		m_rotPowMinusBtn.SetActive(false);

		m_backBtn.SetActive(false);
		m_nextBtn.SetActive(false);
	}
}