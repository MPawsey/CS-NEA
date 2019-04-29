#include "SettingsMenu.h"
#include "Window.h"
#include <algorithm>
#include "EvolutionManager.h"
#include "MenuManager.h"
#include "Functions.h"

namespace Menu
{
	// Initialises the settings menu
	void SettingsMenu::Init()
	{
		// Gets the window instance for the program and the font for the UI
		sf::RenderWindow& window = Window::GetWindow();
		const sf::Font& font = UI::GetFont();

		// Initialises the settings view
		m_settingsView = window.getDefaultView();

		// Positioning variables for UI
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


		/* The below initialises each of the buttons and text fields
		 * for the settings menu using the below layout.
		 * yGap is increased after each row of buttons have been initialised.
		 * 
		 * 
		 * WIDTH		RAYCOUNT	ENGINE POW
		 * HEIGHT		RAY SIZE	ROTATION POW
		 * MUTATE SIZE	MUTATE PC	SPLICE PC
		 *				SEED
		 * 
		 */

		// WIDTH
		m_widthText = sf::Text{ "Car width", font };
		m_widthText.setPosition(xPos1, yPos);
		m_widthMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_widthMinusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_widthMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthMinusBtn.SetCentreText(true);
		m_widthMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) - 0.5, m_widthMinVal, m_widthMaxVal))); });
		m_widthTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_widthTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_widthTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_widthTF, m_widthMinVal, m_widthMaxVal); });
		m_widthTF.SetTooltipText("[15.0-25.0] Default=15\nWidth of the car.");
		m_widthPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_widthPlusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_widthPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_widthPlusBtn.SetCentreText(true);
		m_widthPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_widthTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_widthTF.GetRawText().toAnsiString()) + 0.5, m_widthMinVal, m_widthMaxVal))); });
		
		// RAY COUNT
		m_rayCountText = sf::Text{ "Ray count", font };
		m_rayCountMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_rayCountMinusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_rayCountMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountMinusBtn.SetCentreText(true);
		m_rayCountMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) - 1, m_rayCountMinVal, m_rayCountMaxVal))); });
		m_rayCountText.setPosition(xPos2, yPos);
		m_rayCountTF = UI::UITextField{ textFieldWidth, UI::UITextField::Integer, m_settingsView };
		m_rayCountTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_rayCountTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldIntegerValue(m_rayCountTF, m_rayCountMinVal, m_rayCountMaxVal); });
		m_rayCountTF.SetTooltipText("[3-7] Default=5\nThe number of detector rays\nthat the car uses to detect walls.");
		m_rayCountPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_rayCountPlusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_rayCountPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rayCountPlusBtn.SetCentreText(true);
		m_rayCountPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rayCountTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_rayCountTF.GetRawText().toAnsiString()) + 1, m_rayCountMinVal, m_rayCountMaxVal))); });

		// ENGINE POWER
		m_enginePowText = sf::Text{ "Engine power", font };
		m_enginePowText.setPosition(xPos3, yPos);
		m_enginePowMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_enginePowMinusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_enginePowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowMinusBtn.SetCentreText(true);
		m_enginePowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) - 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });
		m_enginePowTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_enginePowTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_enginePowTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_enginePowTF, m_enginePowMinVal, m_enginePowMaxVal); });
		m_enginePowTF.SetTooltipText("[5.0-15.0] Default=10.0\nThe power of the engine.\ni.e the max speed of the car.");
		m_enginePowPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_enginePowPlusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_enginePowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_enginePowPlusBtn.SetCentreText(true);
		m_enginePowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_enginePowTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_enginePowTF.GetRawText().toAnsiString()) + 0.5, m_enginePowMinVal, m_enginePowMaxVal))); });

		yPos += yGap;

		// HEIGHT
		m_heightText = sf::Text{ "Car height", font };
		m_heightText.setPosition(xPos1, yPos);
		m_heightMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_heightMinusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_heightMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightMinusBtn.SetCentreText(true);
		m_heightMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) - 0.5, m_heightMinVal, m_heightMaxVal))); });
		m_heightTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_heightTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_heightTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_heightTF, m_heightMinVal, m_heightMaxVal); });
		m_heightTF.SetTooltipText("[15.0-25.0] Default=25\nHeight of the car.");
		m_heightPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_heightPlusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_heightPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_heightPlusBtn.SetCentreText(true);
		m_heightPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_heightTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_heightTF.GetRawText().toAnsiString()) + 0.5, m_heightMinVal, m_heightMaxVal))); });

		// RAY SIZE
		m_raySizeText = sf::Text{ "Ray size", font };
		m_raySizeText.setPosition(xPos2, yPos);
		m_raySizeMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_raySizeMinusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_raySizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_raySizeMinusBtn.SetCentreText(true);
		m_raySizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_raySizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_raySizeTF.GetRawText().toAnsiString()) - 25, m_raySizeMinVal, m_raySizeMaxVal))); });
		m_raySizeTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_raySizeTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_raySizeTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_raySizeTF, m_raySizeMinVal, m_raySizeMaxVal); });
		m_raySizeTF.SetTooltipText("[100.0-250.0] Default=150\nThe size of the detector rays.");
		m_raySizePlusBtn = UI::UIButton{ "+", m_settingsView };
		m_raySizePlusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_raySizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_raySizePlusBtn.SetCentreText(true);
		m_raySizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_raySizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_raySizeTF.GetRawText().toAnsiString()) + 25, m_raySizeMinVal, m_raySizeMaxVal))); });

		// ROTATION POWER
		m_rotPowText = sf::Text{ "Turning power", font };
		m_rotPowText.setPosition(xPos3, yPos);
		m_rotPowMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_rotPowMinusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_rotPowMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowMinusBtn.SetCentreText(true);
		m_rotPowMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) - 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });
		m_rotPowTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_rotPowTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_rotPowTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_rotPowTF, m_rotPowMinVal, m_rotPowMaxVal); });
		m_rotPowTF.SetTooltipText("[0.5-2.5] Default=1.0\nThe rotational power of the car.\ni.e the max rotational speed.");
		m_rotPowPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_rotPowPlusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_rotPowPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_rotPowPlusBtn.SetCentreText(true);
		m_rotPowPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_rotPowTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_rotPowTF.GetRawText().toAnsiString()) + 0.5, m_rotPowMinVal, m_rotPowMaxVal))); });

		yPos += yGap;

		// MUTATE SIZE
		m_mutSizeText = sf::Text{ "Mutation size", font };
		m_mutSizeText.setPosition(xPos1, yPos);
		m_mutSizeMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_mutSizeMinusBtn.setPosition(xPos1, yPos + topPadding + yLineSpace);
		m_mutSizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutSizeMinusBtn.SetCentreText(true);
		m_mutSizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_mutSizeTF.GetRawText().toAnsiString()) - 0.1, m_mutSizeMinVal, m_mutSizeMaxVal))); });
		m_mutSizeTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_mutSizeTF.setPosition(xPos1 + buttonWidth + xGap, yPos + yLineSpace);
		m_mutSizeTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_mutSizeTF, m_mutSizeMinVal, m_mutSizeMaxVal); });
		m_mutSizeTF.SetTooltipText("[0.1-2.5] Default=1.0\nThe variation in weights/biases\ndue to mutation.");
		m_mutSizePlusBtn = UI::UIButton{ "+", m_settingsView };
		m_mutSizePlusBtn.setPosition(xPos1 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_mutSizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutSizePlusBtn.SetCentreText(true);
		m_mutSizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_mutSizeTF.GetRawText().toAnsiString()) + 0.1, m_mutSizeMinVal, m_mutSizeMaxVal))); });

		// MUTATE PC
		m_mutPCText = sf::Text{ "Mutate %", font };
		m_mutPCText.setPosition(xPos2, yPos);
		m_mutPCMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_mutPCMinusBtn.setPosition(xPos2, yPos + topPadding + yLineSpace);
		m_mutPCMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutPCMinusBtn.SetCentreText(true);
		m_mutPCMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutPCTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_mutPCTF.GetRawText().toAnsiString()) - 5.0, m_mutPCMinVal, m_mutPCMaxVal))); });
		m_mutPCTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_mutPCTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_mutPCTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_mutPCTF, m_mutPCMinVal, m_mutPCMaxVal); });
		m_mutPCTF.SetTooltipText("[0.0-100.0] Default=25.0\nThe percentage a weight in the\nneural network will mutate.");
		m_mutPCPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_mutPCPlusBtn.setPosition(xPos2 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_mutPCPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_mutPCPlusBtn.SetCentreText(true);
		m_mutPCPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_mutPCTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_mutPCTF.GetRawText().toAnsiString()) + 5.0, m_mutPCMinVal, m_mutPCMaxVal))); });

		// SPLICE PC
		m_splicePCText = sf::Text{ "Splice %", font };
		m_splicePCText.setPosition(xPos3, yPos);
		m_splicePCMinusBtn = UI::UIButton{ "-", m_settingsView };
		m_splicePCMinusBtn.setPosition(xPos3, yPos + topPadding + yLineSpace);
		m_splicePCMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_splicePCMinusBtn.SetCentreText(true);
		m_splicePCMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_splicePCTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_splicePCTF.GetRawText().toAnsiString()) - 5.0, m_splicePCMinVal, m_splicePCMaxVal))); });
		m_splicePCTF = UI::UITextField{ textFieldWidth, UI::UITextField::Decimal, m_settingsView };
		m_splicePCTF.setPosition(xPos3 + buttonWidth + xGap, yPos + yLineSpace);
		m_splicePCTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldDoubleValue(m_splicePCTF, m_splicePCMinVal, m_splicePCMaxVal); });
		m_splicePCTF.SetTooltipText("[0.0-100.0] Default=60\nThe percentage the weights\n between two cars swap\n when reproducing.");
		m_splicePCPlusBtn = UI::UIButton{ "+", m_settingsView };
		m_splicePCPlusBtn.setPosition(xPos3 + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_splicePCPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_splicePCPlusBtn.SetCentreText(true);
		m_splicePCPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_splicePCTF.SetRawText(Functions::DoubleToString(std::clamp(std::stod(m_splicePCTF.GetRawText().toAnsiString()) + 5.0, m_splicePCMinVal, m_splicePCMaxVal))); });

		yPos += yGap;

		// SEED
		m_seedText = sf::Text{ "Seed", font };
		m_seedText.setPosition(xPos2 + buttonWidth + xGap, yPos);
		m_seedTF = UI::UITextField{ textFieldWidth, UI::UITextField::Integer, m_settingsView };
		m_seedTF.setPosition(xPos2 + buttonWidth + xGap, yPos + yLineSpace);
		m_seedTF.SetTooltipText("The seed to use for the\nrandom number generator in the\nsimulation. Leave blank if a random\nseed is wanted.");
		m_seedTF.GetLostFocusEvent().AddCallback([&]() { if (m_seedTF.GetRawText().getSize() > 18) m_seedTF.SetRawText(m_seedTF.GetRawText().substring(0, 18)); });


		// Initialises the back button
		m_backBtn = UI::UIButton{ "Back", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(xPos1, window.getSize().y - yLineSpace);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { Evolution::EvolutionManager::GetEvolutionManager().Reset(false); MenuManager::GetMenuManager().GoToState(MenuState::MainMenu); });
		
		// Initialises the next button
		m_nextBtn = UI::UIButton{ "Next", m_settingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_nextBtn.setPosition(675.f, window.getSize().y - yLineSpace);
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::StartMap); });

		// Initialises the advanced settings button
		m_advSettingsBtn = UI::UIButton{ "Advanced", m_settingsView, { 2.f, 5.f, 0.f, 0.f } };
		m_advSettingsBtn.setPosition(xPos2, window.getSize().y - yLineSpace);
		m_advSettingsBtn.SetCentreText(true);
		m_advSettingsBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::AdvancedConfig); });


		// Sets the default values for the text fields
		m_widthTF.SetRawText("15");
		m_heightTF.SetRawText("25");
		m_rayCountTF.SetRawText("5");
		m_raySizeTF.SetRawText("150");
		m_mutSizeTF.SetRawText("1.0");
		m_enginePowTF.SetRawText("10");
		m_rotPowTF.SetRawText("1");
		m_mutPCTF.SetRawText("25");
		m_splicePCTF.SetRawText("60");
		m_seedTF.SetRawText("");

	}

	// Updates the settings menu (includes drawing to the window)
	void SettingsMenu::Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Draws the width stuff
		window.draw(m_widthText);
		window.draw(m_widthPlusBtn);
		window.draw(m_widthTF);
		window.draw(m_widthMinusBtn);

		// Draws the height stuff
		window.draw(m_heightText);
		window.draw(m_heightPlusBtn);
		window.draw(m_heightTF);
		window.draw(m_heightMinusBtn);

		// Draws the ray count stuff
		window.draw(m_rayCountText);
		window.draw(m_rayCountPlusBtn);
		window.draw(m_rayCountTF);
		window.draw(m_rayCountMinusBtn);

		// Draws the ray size stuff
		window.draw(m_raySizeText);
		window.draw(m_raySizePlusBtn);
		window.draw(m_raySizeTF);
		window.draw(m_raySizeMinusBtn);

		// Draws the mutation size stuff
		window.draw(m_mutSizeText);
		window.draw(m_mutSizePlusBtn);
		window.draw(m_mutSizeTF);
		window.draw(m_mutSizeMinusBtn);

		// Draws the engine power stuff
		window.draw(m_enginePowText);
		window.draw(m_enginePowPlusBtn);
		window.draw(m_enginePowTF);
		window.draw(m_enginePowMinusBtn);

		// Draws the rotation power stuff
		window.draw(m_rotPowText);
		window.draw(m_rotPowPlusBtn);
		window.draw(m_rotPowTF);
		window.draw(m_rotPowMinusBtn);

		// Draws the mutation chance stuff
		window.draw(m_mutPCText);
		window.draw(m_mutPCPlusBtn);
		window.draw(m_mutPCTF);
		window.draw(m_mutPCMinusBtn);

		// Draws the splicing chance stuff
		window.draw(m_splicePCText);
		window.draw(m_splicePCPlusBtn);
		window.draw(m_splicePCTF);
		window.draw(m_splicePCMinusBtn);

		// Draws the seed stuff
		window.draw(m_seedText);
		window.draw(m_seedTF);
		
		// Draws the back, next and advanced settings buttons
		window.draw(m_backBtn);
		window.draw(m_nextBtn);
		window.draw(m_advSettingsBtn);
	}

	// Loads the settings menu
	void SettingsMenu::Load()
	{
		// Sets the window view to the settings view
		Window::GetWindow().setView(m_settingsView);

		// Sets all of the UI elements activity states to true
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

		m_mutSizePlusBtn.SetActive(true);
		m_mutSizeTF.SetActive(true);
		m_mutSizeMinusBtn.SetActive(true);

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
		m_advSettingsBtn.SetActive(true);
	}

	// Unloads the settings menu
	void SettingsMenu::Unload()
	{
		// Saves the current settings for the generation if the menu is currently on the map select menu
		if (Menu::MenuManager::GetMenuManager().GetMenuState() == Menu::MenuState::StartMap)
			Evolution::EvolutionManager::GetEvolutionManager().CreateGenerationFromSettings(m_widthTF.GetFloatValue(), m_heightTF.GetFloatValue(), m_rayCountTF.GetUIntegerValue(), m_raySizeTF.GetFloatValue(),
				m_mutSizeTF.GetDoubleValue(), m_enginePowTF.GetFloatValue(), m_rotPowTF.GetFloatValue(), m_mutPCTF.GetDoubleValue() / 100.0, m_splicePCTF.GetDoubleValue() / 100.0, m_seedTF.GetRawText() == "" ? std::random_device{}() : m_seedTF.GetUIntegerValue());

		// Sets all of the UI elements activity states to false
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

		m_mutSizePlusBtn.SetActive(false);
		m_mutSizeTF.SetActive(false);
		m_mutSizeMinusBtn.SetActive(false);

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
		m_advSettingsBtn.SetActive(false);
	}
}