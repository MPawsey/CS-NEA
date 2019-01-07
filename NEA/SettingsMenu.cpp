#include "SettingsMenu.h"
#include "UI.h"
#include "Window.h"
#include <algorithm>

namespace Menu::SettingsMenu
{
	// Private
	sf::View m_settingsView;

	std::string DoubleToString(double d)
	{
		std::string s = std::to_string(d);
		return s.erase(s.find_last_not_of('0') + 1, std::string::npos).back() == '.' ? (s.pop_back(), s) : s;
	}
	
	void ClampTextFieldValue(UI::TextField& text, double boundLower, double boundUpper)
	{
		text.SetRawText(DoubleToString(std::clamp(std::stod(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	// WIDTH
	constexpr double m_widthMinVal = 15.0;
	constexpr double m_widthMaxVal = 25.0;
	sf::Text m_widthLabel;
	UI::Button m_plusWidthBtn;
	UI::Button m_minusWidthBtn;
	UI::TextField m_widthTF;

	// HEIGHT
	sf::Text m_heightLabel;
	UI::Button m_plusHeightBtn;
	UI::Button m_minusHeightBtn;
	UI::TextField m_heightTF;

	// RAY COUNT
	sf::Text m_rayCountLabel;
	UI::Button m_plusRayCountBtn;
	UI::Button m_minusRayCountBtn;
	UI::TextField m_rayCountTF;

	// POPULATION SIZE
	sf::Text m_popSizeLabel;
	UI::Button m_plusPopSizeBtn;
	UI::Button m_minusPopSizeBtn;
	UI::TextField m_popSizeTF;

	// ENGINE POWER
	sf::Text m_enginePowLabel;
	UI::Button m_plusEnginePowBtn;
	UI::Button m_minusEnginePowBtn;
	UI::TextField m_enginePowTF;

	// ROTATION POWER
	sf::Text m_rotPowLabel;
	UI::Button m_plusRotPowBtn;
	UI::Button m_minusRotPowBtn;
	UI::TextField m_rotPowTF;

	// Public

	void Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_settingsView = window.getDefaultView();
		const sf::Font& font = UI::GetFont();

		float xPos1 = 50.f;
		float xPos2 = 450.f;
		float yPos = 50.f;
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yGap = 175.f;
		float xGap = 10.f;
		float textFieldWidth = 100.f;
		float buttonWidth = 35.f;

		// WIDTH
		m_widthLabel = sf::Text{ "Car width", font };
		m_widthLabel.setPosition(xPos1, yPos);
		m_plusWidthBtn = UI::Button{ sf::Vector2f{ xPos1, yPos + yLineSpace }, m_settingsView, "+" };
		m_plusWidthBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_plusWidthBtn.SetCentreText(true);
		m_plusWidthBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) + 0.5, m_widthMinVal, m_widthMaxVal))); });
		m_widthTF = UI::TextField{ sf::Vector2f{ xPos1 + buttonWidth + xGap, yPos + yLineSpace }, m_settingsView, textFieldWidth, UI::TextField::Decimal };
		m_widthTF.GetLostFocusEvent().AddCallback([&]() { ClampTextFieldValue(m_widthTF, m_widthMinVal, m_widthMaxVal); });
		m_minusWidthBtn = UI::Button{ sf::Vector2f{ xPos1 + buttonWidth + xGap + textFieldWidth + xGap, yPos + yLineSpace }, m_settingsView, "-" };
		m_minusWidthBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_minusWidthBtn.SetCentreText(true);
		m_minusWidthBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) - 0.5, m_widthMinVal, m_widthMaxVal))); });


		// HEIGHT
		m_heightLabel = sf::Text{ "Car height", font };
		m_heightLabel.setPosition(xPos2, yPos);


		yPos += yGap;

		// RAY COUNT
		m_rayCountLabel = sf::Text{ "Ray count", font };
		m_rayCountLabel.setPosition(xPos1, yPos);


		// POPULATION SIZE
		m_popSizeLabel = sf::Text{ "Population size", font };
		m_popSizeLabel.setPosition(xPos2, yPos);


		yPos += yGap;

		// ENGINE POWER
		m_enginePowLabel = sf::Text{ "Engine power", font };
		m_enginePowLabel.setPosition(xPos1, yPos);


		// ROTATION POWER
		m_rotPowLabel = sf::Text{ "Rotation power", font };
		m_rotPowLabel.setPosition(xPos2, yPos);

	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();


		window.draw(m_widthLabel);
		window.draw(m_plusWidthBtn);
		window.draw(m_widthTF);
		window.draw(m_minusWidthBtn);

		window.draw(m_heightLabel);
		//window.draw(*m_plusHeightBtn);
		//window.draw(*m_minusHeightBtn);
		//window.draw(*m_heightTF);

		window.draw(m_rayCountLabel);
		//window.draw(*m_plusRayCountBtn);
		//window.draw(*m_minusRayCountBtn);
		//window.draw(*m_rayCountTF);

		window.draw(m_popSizeLabel);
		//window.draw(*m_plusPopSizeBtn);
		//window.draw(*m_minusPopSizeBtn);
		//window.draw(*m_popSizeTF);

		window.draw(m_enginePowLabel);
		//window.draw(*m_plusEnginePowBtn);
		//window.draw(*m_minusEnginePowBtn);
		//window.draw(*m_enginePowTF);

		window.draw(m_rotPowLabel);
		//window.draw(*m_plusRotPowBtn);
		//window.draw(*m_minusRotPowBtn);
		//window.draw(*m_rotPowTF);*/
	}

	void Load()
	{
		Window::GetWindow().setView(m_settingsView);

		m_plusWidthBtn.SetActive(true);
		m_widthTF.SetActive(true);
		m_minusWidthBtn.SetActive(true);


		//m_plusHeightBtn->SetActive(true);
		//m_minusHeightBtn->SetActive(true);
		//m_heightTF->SetActive(true);

		//m_plusRayCountBtn->SetActive(true);
		//m_minusRayCountBtn->SetActive(true);
		//m_rayCountTF->SetActive(true);

		//m_plusPopSizeBtn->SetActive(true);
		//m_minusPopSizeBtn->SetActive(true);
		//m_popSizeTF->SetActive(true);

		//m_plusEnginePowBtn->SetActive(true);
		//m_minusEnginePowBtn->SetActive(true);
		//m_enginePowTF->SetActive(true);

		//m_plusRotPowBtn->SetActive(true);
		//m_minusRotPowBtn->SetActive(true);
		//m_rotPowTF->SetActive(true);

		m_widthTF.SetRawText("25");
		//m_heightTF->SetRawText("50");
		//m_rayCountTF->SetRawText("5");
		//m_popSizeTF->SetRawText("25");
		//m_enginePowTF->SetRawText("5");
		//m_rotPowTF->SetRawText("5");*/
	}

	void Unload()
	{
		m_plusWidthBtn.SetActive(false);
		m_widthTF.SetActive(false);
		m_minusWidthBtn.SetActive(false);


		//m_plusHeightBtn->SetActive(false);
		//m_minusHeightBtn->SetActive(false);
		//m_heightTF->SetActive(false);

		//m_plusRayCountBtn->SetActive(false);
		//m_minusRayCountBtn->SetActive(false);
		//m_rayCountTF->SetActive(false);

		//m_plusPopSizeBtn->SetActive(false);
		//m_minusPopSizeBtn->SetActive(false);
		//m_popSizeTF->SetActive(false);

		//m_plusEnginePowBtn->SetActive(false);
		//m_minusEnginePowBtn->SetActive(false);
		//m_enginePowTF->SetActive(false);

		//m_plusRotPowBtn->SetActive(false);
		//m_minusRotPowBtn->SetActive(false);
		//m_rotPowTF->SetActive(false);*/
	}
}