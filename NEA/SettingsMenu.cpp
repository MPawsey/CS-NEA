#include "SettingsMenu.h"
#include "UI.h"
#include "Window.h"

namespace Menu::SettingsMenu
{
	// Private
	sf::View m_settingsView;
	
	// WIDTH
	/*sf::Text m_widthLabel;
	UI::Button* m_plusWidthBtn;
	UI::Button* m_minusWidthBtn;
	UI::TextField* m_widthTF;

	// HEIGHT
	sf::Text m_heightLabel;
	UI::Button* m_plusHeightBtn;
	UI::Button* m_minusHeightBtn;
	UI::TextField* m_heightTF;

	// RAY COUNT
	sf::Text m_rayCountLabel;
	UI::Button* m_plusRayCountBtn;
	UI::Button* m_minusRayCountBtn;
	UI::TextField* m_rayCountTF;

	// POPULATION SIZE
	sf::Text m_popSizeLabel;
	UI::Button* m_plusPopSizeBtn;
	UI::Button* m_minusPopSizeBtn;
	UI::TextField* m_popSizeTF;

	// ENGINE POWER
	sf::Text m_enginePowLabel;
	UI::Button* m_plusEnginePowBtn;
	UI::Button* m_minusEnginePowBtn;
	UI::TextField* m_enginePowTF;

	// ROTATION POWER
	sf::Text m_rotPowLabel;
	UI::Button* m_plusRotPowBtn;
	UI::Button* m_minusRotPowBtn;
	UI::TextField* m_rotPowTF;*/



	void OnWindowClosedEvent()
	{
		// WIDTH
		/*delete m_plusWidthBtn;
		delete m_minusWidthBtn;
		delete m_widthTF;

		// HEIGHT
		delete m_plusHeightBtn;
		delete m_minusHeightBtn;
		delete m_heightTF;

		// RAY COUNT
		delete m_plusRayCountBtn;
		delete m_minusRayCountBtn;
		delete m_rayCountTF;

		// POPULATION SIZE
		delete m_plusPopSizeBtn;
		delete m_minusPopSizeBtn;
		delete m_popSizeTF;

		// ENGINE POWER
		delete m_plusEnginePowBtn;
		delete m_minusEnginePowBtn;
		delete m_enginePowTF;

		// ROTATION POWER
		delete m_plusRotPowBtn;
		delete m_minusRotPowBtn;
		delete m_rotPowTF;*/
	}

	// Public

	void Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_settingsView = window.getDefaultView();
		const sf::Font& font = UI::GetFont();

		float xStart1 = 50.f;
		float xStart2 = 350.f;
		float yStart = 100.f;
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yGap = 150.f - yLineSpace;
		float xGap = 5.f;


		// WIDTH
		/*m_widthLabel = sf::Text{ "Width", font };
		m_widthLabel.setPosition(sf::Vector2f{ xStart1, yStart });
		m_plusWidthBtn = new UI::Button{ sf::Vector2f{ xStart1, yStart + yLineSpace }, m_settingsView, "+", sf::Vector2f{ 5.f, 0.f } };
		m_widthTF = new UI::TextField{ sf::Vector2f{ xStart1 + m_plusWidthBtn->GetBounds().width + xGap, yStart + yLineSpace }, m_settingsView, 100.f, UI::TextField::Decimal };

		// HEIGHT
		m_heightLabel = sf::Text{ "Height", font };
		m_heightLabel.setPosition(sf::Vector2f{ xStart2, yStart });

		yStart += yGap;

		// RAY COUNT
		m_rayCountLabel = sf::Text{ "Ray Count", font };
		m_rayCountLabel.setPosition(sf::Vector2f{ xStart1, yStart });

		// POPULATION SIZE
		m_popSizeLabel = sf::Text{ "Population Size", font };
		m_popSizeLabel.setPosition(sf::Vector2f{ xStart2, yStart });

		yStart += yGap;

		// ENGINE POWER
		m_enginePowLabel = sf::Text{ "Engine Power", font };
		m_enginePowLabel.setPosition(sf::Vector2f{ xStart1, yStart });

		// ROTATION POWER
		m_rotPowLabel = sf::Text{ "Rotation Power", font };
		m_rotPowLabel.setPosition(sf::Vector2f{ xStart2, yStart });*/
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		/*window.draw(m_widthLabel);
		window.draw(*m_plusWidthBtn);
		window.draw(*m_widthTF);
		//window.draw(*m_minusWidthBtn);

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

		/*m_plusWidthBtn->SetActive(true);
		m_widthTF->SetActive(true);
		//m_minusWidthBtn->SetActive(true);


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

		//m_widthTF->SetRawText("25.0");
		//m_heightTF->SetRawText("50.0");
		//m_rayCountTF->SetRawText("5");
		//m_popSizeTF->SetRawText("25");
		//m_enginePowTF->SetRawText("5.0");
		//m_rotPowTF->SetRawText("5.0");*/
	}

	void Unload()
	{
		/*m_plusWidthBtn->SetActive(false);
		m_widthTF->SetActive(false);
		//m_minusWidthBtn->SetActive(false);


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