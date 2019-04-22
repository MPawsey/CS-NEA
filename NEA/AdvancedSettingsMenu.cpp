#include "AdvancedSettingsMenu.h"
#include <sfml/Graphics.hpp>
#include "Window.h"
#include "MenuManager.h"
#include "EvolutionManager.h"
#include <algorithm>
#include "InputManager.h"
#include "Functions.h"
#include "Colours.h"

namespace Menu
{

	// Event for when onlyPairOnce checkbox is updated
	// Sets the save and kill size to be the same if checked
	void AdvancedSettingsMenu::OnPairCBUpdate(bool isChecked)
	{
		// Only has to do anything if the checkbox is checked
		if (isChecked)
		{
			// Gets the population size
			unsigned int popSize = m_popSizeTF.GetUIntegerValue();
			
			// Limits the save size to be at max half the population size
			if (m_saveTF.GetUIntegerValue() > popSize / 2)
			{
				m_saveTF.SetRawText(std::to_string(popSize / 2));
			}
			// Sets the kill size to be the same as the save size
			m_killTF.SetRawText(m_saveTF.GetRawText());
		}
	}

	// Checks to see if the save/kill size TF are valid
	// Fixes them if they are not
	// Called during their loss of focus event
	void AdvancedSettingsMenu::CheckTFValues(bool isSaveTF)
	{
		// Gets the population size
		unsigned int popSize = m_popSizeTF.GetUIntegerValue();

		// Limits the upper bound of the fields
		if (m_saveTF.GetUIntegerValue() > popSize)
		{
			m_saveTF.SetRawText(std::to_string(popSize));
		}
		if (m_killTF.GetUIntegerValue() > popSize - 2) // Ensures that at least two cars exist
		{
			m_killTF.SetRawText(std::to_string(popSize - 2));
		}

		// Updates the other text field if the onlyPairOnce checkbox is checked
		if (m_onlyPairOnceCB.IsChecked())
		{
			// Checks to see which field called this subroutine
			if (isSaveTF)
			{
				// Checks to see if the save size will exceed the population size
				// when doubled (as kill size = save size)
				if (m_saveTF.GetUIntegerValue() * 2 > popSize)
				{
					// Sets the save/kill size to half of the population size
					m_saveTF.SetRawText(std::to_string(popSize / 2));
					m_killTF.SetRawText(std::to_string(popSize / 2));
				}
				else
				{
					// Sets the kill size to the save size
					m_killTF.SetRawText(m_saveTF.GetRawText());
				}
			}
			else
			{
				// Checks to see if the kill size will exceed the population size
				// when doubled (as save size = kill size)
				if (m_killTF.GetUIntegerValue() * 2 > popSize)
				{
					// Sets the save/kill size to half of the population size
					m_saveTF.SetRawText(std::to_string(popSize / 2));
					m_killTF.SetRawText(std::to_string(popSize / 2));
				}
				else
				{
					// Sets the save size to the save size
					m_saveTF.SetRawText(m_killTF.GetRawText());
				}
			}
		}
	}

	// Event for when the scroll wheel is used
	void AdvancedSettingsMenu::OnMouseScrolled(int delta)
	{
		// Updates the layer slider position if its active and can be used
		if (m_layerSlider.IsActive() && m_layerSliderMax > 0)
			m_layerSlider.Move(-delta * ((UI::GetFont().getLineSpacing(30) + 20.f) / m_layerSliderMax));
	}

	// Initialises the advanced settings menu
	void AdvancedSettingsMenu::Init()
	{
		// Gets the font for the UI
		const sf::Font& font = UI::GetFont();

		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Sets the starting positioning variables
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yPos = 50.f;
		float yGap = 125.f;
		float xGap = 10.f;
		float textFieldWidth = 100.f;
		float buttonWidth = 35.f;
		float topPadding = 2.f, hzPadding = 4.f;

		// Initialises the advanced settings view
		m_advSettingsView = Window::GetDefaultWindowView();

		// Initialises the back button
		m_backBtn = UI::UIButton{ "Back", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - yLineSpace);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::StartConfig); });

		// Initialises the population Size
		m_popSizeText = sf::Text{ "Population size", font };
		m_popSizeText.setPosition(50.f, yPos);
		m_popSizeMinusBtn = UI::UIButton{ "-", m_advSettingsView };
		m_popSizeMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_popSizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizeMinusBtn.SetCentreText(true);
		m_popSizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) - 1, m_popSizeMinVal, m_popSizeMaxVal))); CheckTFValues(true); });
		m_popSizeTF = UI::UITextField{ textFieldWidth, UI::UITextField::Integer, m_advSettingsView };
		m_popSizeTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_popSizeTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldIntegerValue(m_popSizeTF, m_popSizeMinVal, m_popSizeMaxVal); CheckTFValues(true); });
		m_popSizeTF.SetTooltipText("[15-35] Default=25\nThe number of cars in each generation.");
		m_popSizePlusBtn = UI::UIButton{ "+", m_advSettingsView };
		m_popSizePlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_popSizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizePlusBtn.SetCentreText(true);
		m_popSizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) + 1, m_popSizeMinVal, m_popSizeMaxVal))); CheckTFValues(true); });

		// Updates the ygap for the UI on the left side of the screen
		yPos += yGap;

		// Initialises the multi Reproduce
		m_onlyPairOnceText.setFont(font);
		m_onlyPairOnceText.setString("Only Pair Once");
		m_onlyPairOnceText.setPosition(50.f, yPos);
		m_onlyPairOnceCB = UI::UICheckBox{ m_advSettingsView };
		m_onlyPairOnceCB.setPosition(125.f, yPos + yLineSpace);
		m_onlyPairOnceCB.SetTooltipText("If checked, the reproduction stage will\nonly allow each car to create 1 offspring\neach (two per pair).\nThis will prevent the ability to choose the\na different number of saved and\nkilled cars.");
		m_onlyPairOnceCB.GetCheckBoxUpdateEvent().AddCallback(&AdvancedSettingsMenu::OnPairCBUpdate, *this);

		yPos += yGap;

		// Initialises the  save Size
		m_saveText = sf::Text{ "Save size", font };
		m_saveText.setPosition(50.f, yPos);
		m_saveMinusBtn = UI::UIButton{ "-", m_advSettingsView };
		m_saveMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_saveMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_saveMinusBtn.SetCentreText(true);
		m_saveMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_saveTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_saveTF.GetRawText().toAnsiString()) - 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(true); });
		m_saveTF = UI::UITextField{ textFieldWidth, UI::UITextField::Integer, m_advSettingsView };
		m_saveTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_saveTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(true); });
		m_saveTF.SetTooltipText("Sets the number of cars that will\nbe copied to the next generation\nat the end of each generation.");
		m_savePlusBtn = UI::UIButton{ "+", m_advSettingsView };
		m_savePlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_savePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_savePlusBtn.SetCentreText(true);
		m_savePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_saveTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_saveTF.GetRawText().toAnsiString()) + 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(true); });

		yPos += yGap;

		// Initialises the  kill Size
		m_killText = sf::Text{ "Kill size", font };
		m_killText.setPosition(50.f, yPos);
		m_killMinusBtn = UI::UIButton{ "-", m_advSettingsView };
		m_killMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_killMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_killMinusBtn.SetCentreText(true);
		m_killMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_killTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_killTF.GetRawText().toAnsiString()) - 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(false); });
		m_killTF = UI::UITextField{ textFieldWidth, UI::UITextField::Integer, m_advSettingsView };
		m_killTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_killTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(false); });
		m_killTF.SetTooltipText("Sets the number of cars that will\nbe deleted to the next generation\nat the end of each generation.");
		m_killPlusBtn = UI::UIButton{ "+", m_advSettingsView };
		m_killPlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_killPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_killPlusBtn.SetCentreText(true);
		m_killPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_killTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_killTF.GetRawText().toAnsiString()) + 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(false); });

		// Initialises the NN layers
		m_layerView.setViewport(sf::FloatRect{ 0.7f, 0.35f, 0.2f, 0.6f });
		m_layerView.setSize(window.getSize().x * 0.2f, window.getSize().y * 0.6f);
		m_layerView.setCenter(m_layerView.getSize() / 2.f);

		m_layersText.setFont(font);
		m_layersText.setCharacterSize(20);
		m_layersText.setString("Hidden Layers\nThese text fields control the hidden layers\nin the neural network. Adding/removing\na layer will always happen on the last layer.\nThe hidden layers are the layers between\nthe input and output layers in a nn.");
		m_layersText.setPosition(window.getSize().x * 0.5f, 50.f);

		m_layersBackground.setFillColor(Colours::UI_BLUE_BG);
		m_layersBackground.setPosition(window.getSize().x * 0.7f, window.getSize().y * 0.35f);
		m_layersBackground.setSize(sf::Vector2f{ window.getSize().x * 0.2f, window.getSize().y * 0.6f });

		// Creates the two starting layer size TFs and adds them to the layers list
		UI::UITextField* layerTF = new UI::UITextField{ 100.f, UI::UITextField::Integer, m_layerView };
		layerTF->setPosition(25.f, 25.f);
		layerTF->SetRawText("4");
		layerTF->GetLostFocusEvent().AddCallback([=]() { if (layerTF->GetUIntegerValue() > 25) layerTF->SetRawText("25"); }); // Creates the upper bound as 25
		m_layerSizes.push_back(layerTF);
		UI::UITextField* layer2TF = new UI::UITextField{ 100.f, UI::UITextField::Integer, m_layerView };
		layer2TF->setPosition(25.f, 25.f + yLineSpace + 15.f);
		layer2TF->SetRawText("3");
		layer2TF->GetLostFocusEvent().AddCallback([=]() { if (layer2TF->GetUIntegerValue() > 25) layer2TF->SetRawText("25"); });
		m_layerSizes.push_back(layer2TF);

		// Initialises the slider for the layers
		m_layerSlider = UI::UISlider{ sf::Vector2f{window.getSize().x * 0.95f, window.getSize().y * 0.35f}, m_advSettingsView, window.getSize().y * 0.6f };
		m_layerSlider.GetSliderUpdateEvent().AddCallback([&](float val)
		{
			// Updates the layer view position when the slider is updated
			if (m_layerSliderMax > 0)
				m_layerView.setCenter(m_layerView.getSize().x / 2.f, (m_layerView.getSize().y / 2.f) + (m_layerSliderMax * val));
		});

		// Initialises the add layer button
		m_addLayerBtn = UI::UIButton{ "Add Layer", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_addLayerBtn.setPosition(window.getSize().x * (1.f - 0.35f) - m_addLayerBtn.GetBounds().width + 10.f, window.getSize().y * 0.65f);
		m_addLayerBtn.SetCentreText(true);
		m_addLayerBtn.GetMouseClickedEvent().AddCallback([&]() 
		{
			// Only adds a layer if there is room (no more than 15)
			if (m_layerSizes.size() > 15)
				return;

			// Creates a new layer text field and initialises it with default settings
			// Positions it after the previous layer TF
			UI::UITextField* layerTF = new UI::UITextField{ 100.f, UI::UITextField::Integer, m_layerView };
			layerTF->setPosition(25.f, 25.f + ((UI::GetFont().getLineSpacing(30) + 20.f) * m_layerSizes.size()));
			layerTF->SetRawText("4");
			layerTF->SetActive(true);
			layerTF->GetLostFocusEvent().AddCallback([=]() { if (layerTF->GetUIntegerValue() > 25) layerTF->SetRawText("25"); });
			m_layerSizes.push_back(layerTF);

			// Updates the slider size
			m_layerSliderMax = (25.f + ((m_layerSizes.size()) * (UI::GetFont().getLineSpacing(30) + 20.f))) - m_layerView.getSize().y;
		});

		// Initialises the remove layer button
		m_removeLayer = UI::UIButton{ "Remove Layer", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_removeLayer.setPosition(window.getSize().x * (1.f - 0.35f) - m_removeLayer.GetBounds().width + 10.f, window.getSize().y * 0.65f + (yLineSpace + 15.f));
		m_removeLayer.SetCentreText(true);
		m_removeLayer.GetMouseClickedEvent().AddCallback([&]() 
		{ 
			// Checks to see if there is a layer TF to remove
			if (m_layerSizes.size() > 0) 
			{ 
				// Delets the layer to free up memory before deleting it from the list
				delete m_layerSizes.back(); 
				m_layerSizes.pop_back();
				
				// Updates the slider size
				m_layerSliderMax = (25.f + ((m_layerSizes.size()) * (UI::GetFont().getLineSpacing(30) + 20.f))) - m_layerView.getSize().y;
			}
		});

		// Sets the starting text for the text fields
		m_popSizeTF.SetRawText("25");
		m_saveTF.SetRawText("0");
		m_killTF.SetRawText("22");

		// Adds a callback for when the scroll wheel is used
		InputManager::GetMouseScrolledEvent().AddCallback(&AdvancedSettingsMenu::OnMouseScrolled, *this);
	}
	
	// Updates the advanced settings menu (includes drawing to window)
	void AdvancedSettingsMenu::Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Sets the view to the advanced settings view
		window.setView(m_advSettingsView);

		// Draws the only pair once stuff
		window.draw(m_onlyPairOnceText);
		window.draw(m_onlyPairOnceCB);

		// Draws the population size stuff
		window.draw(m_popSizeText);
		window.draw(m_popSizePlusBtn);
		window.draw(m_popSizeTF);
		window.draw(m_popSizeMinusBtn);

		// Draws the save size stuff
		window.draw(m_saveText);
		window.draw(m_savePlusBtn);
		window.draw(m_saveTF);
		window.draw(m_saveMinusBtn);

		// Draws the kill size stuff
		window.draw(m_killText);
		window.draw(m_killPlusBtn);
		window.draw(m_killTF);
		window.draw(m_killMinusBtn);

		// Dras the layers stuff
		window.draw(m_layersText);
		window.draw(m_layersBackground);
		window.draw(m_layerSlider);
		window.draw(m_addLayerBtn);
		window.draw(m_removeLayer);

		// Dras the back button
		window.draw(m_backBtn);

		// Sets the window view to the layer view and draws the layer text fields
		window.setView(m_layerView);
		for (auto tf : m_layerSizes)
		{
			window.draw(*tf);
		}
	}
	
	// Loads the advanced settings menu
	void AdvancedSettingsMenu::Load()
	{
		// Sets all of the activity states to true for all of the UI elements
		m_popSizePlusBtn.SetActive(true);
		m_popSizeTF.SetActive(true);
		m_popSizeMinusBtn.SetActive(true);

		m_onlyPairOnceCB.SetActive(true);

		m_savePlusBtn.SetActive(true);
		m_saveTF.SetActive(true);
		m_saveMinusBtn.SetActive(true);

		m_killPlusBtn.SetActive(true);
		m_killTF.SetActive(true);
		m_killMinusBtn.SetActive(true);

		m_layerSlider.SetActive(true);
		m_addLayerBtn.SetActive(true);
		m_removeLayer.SetActive(true);
		for (auto tf : m_layerSizes)
		{
			tf->SetActive(true);
		}

		m_backBtn.SetActive(true);
	}
	
	// Unloads the advanced settings menu
	void AdvancedSettingsMenu::Unload()
	{
		// Creates a vector to store the integer values of the layers
		std::vector<unsigned int> layerSizes;
		// Iterates through the text fields in the layer sizes list
		for (auto tf : m_layerSizes)
		{
			// Converts the text field to an unsigned integer and adds it to
			// the layer sizes vector if it is non-zero
			unsigned int size = tf->GetUIntegerValue();
			if (size != 0)
				layerSizes.push_back(size);
		}
		// Appends 2 to the list (for the final output of the nn)
		layerSizes.push_back(2);
		
		// Applies the settings from the advanced settings menu
		Evolution::EvolutionManager::GetEvolutionManager().SetAdvancedSettings(m_popSizeTF.GetUIntegerValue(), m_saveTF.GetUIntegerValue(), m_killTF.GetUIntegerValue(), !m_onlyPairOnceCB.IsChecked(), layerSizes);

		// Sets all of the activity states to false for all of the UI elements
		m_popSizePlusBtn.SetActive(false);
		m_popSizeTF.SetActive(false);
		m_popSizeMinusBtn.SetActive(false);

		m_onlyPairOnceCB.SetActive(false);

		m_savePlusBtn.SetActive(false);
		m_saveTF.SetActive(false);
		m_saveMinusBtn.SetActive(false);

		m_killPlusBtn.SetActive(false);
		m_killTF.SetActive(false);
		m_killMinusBtn.SetActive(false);

		m_layerSlider.SetActive(false);
		m_addLayerBtn.SetActive(false);
		m_removeLayer.SetActive(false);
		for (auto tf : m_layerSizes)
		{
			tf->SetActive(false);
		}

		m_backBtn.SetActive(false);
	}
}