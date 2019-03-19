#include "AdvancedSettingsMenu.h"
#include <sfml/Graphics.hpp>
#include "Window.h"
#include "MenuManager.h"
#include "EvolutionManager.h"
#include <algorithm>
#include "InputManager.h"
#include "Functions.h"

namespace Menu
{


	void AdvancedSettingsMenu::OnPairCBUpdate(bool isChecked)
	{
		unsigned int popSize = m_popSizeTF.GetUIntegerValue();
		if (isChecked)
		{
			if (m_saveTF.GetUIntegerValue() > popSize / 2)
			{
				m_saveTF.SetRawText(std::to_string(popSize / 2));
			}
			m_killTF.SetRawText(m_saveTF.GetRawText());
		}
	}

	void AdvancedSettingsMenu::CheckTFValues(bool isSaveTF)
	{
		unsigned int popSize = m_popSizeTF.GetUIntegerValue();

		if (m_saveTF.GetUIntegerValue() > popSize)
		{
			m_saveTF.SetRawText(std::to_string(popSize));
		}
		if (m_killTF.GetUIntegerValue() > popSize)
		{
			m_killTF.SetRawText(std::to_string(popSize));
		}

		if (m_onlyPairOnceCB.IsChecked())
		{
			if (isSaveTF)
			{
				if (m_saveTF.GetUIntegerValue() * 2 > popSize)
				{
					m_saveTF.SetRawText(std::to_string(popSize / 2));
					m_killTF.SetRawText(std::to_string(popSize / 2));
				}
				else
				{
					m_killTF.SetRawText(m_saveTF.GetRawText());
				}
			}
			else
			{
				if (m_killTF.GetUIntegerValue() * 2 > popSize)
				{
					m_saveTF.SetRawText(std::to_string(popSize / 2));
					m_killTF.SetRawText(std::to_string(popSize / 2));
				}
				else
				{
					m_saveTF.SetRawText(m_killTF.GetRawText());
				}
			}
		}
		else if (m_saveTF.GetUIntegerValue() + m_killTF.GetUIntegerValue() > popSize)
		{
			if (isSaveTF)
			{
				m_killTF.SetRawText(std::to_string(popSize - m_saveTF.GetUIntegerValue()));
			}
			else
			{
				m_saveTF.SetRawText(std::to_string(popSize - m_killTF.GetUIntegerValue()));
			}
		}
	}


	void AdvancedSettingsMenu::OnMouseScrolled(int delta)
	{
		if (m_layerSlider.IsActive())
			m_layerSlider.Move(-delta * ((UI::GetFont().getLineSpacing(30) + 20.f) / m_layerSliderMax));
	}

	// Public

	void AdvancedSettingsMenu::Init()
	{
		const sf::Font& font = UI::GetFont();
		float yLineSpace = font.getLineSpacing(30) + 5.f;
		float yPos = 50.f;
		float yGap = 125.f;
		float xGap = 10.f;
		float textFieldWidth = 100.f;
		float buttonWidth = 35.f;
		float topPadding = 2.f, hzPadding = 4.f;

		sf::RenderWindow& window = Window::GetWindow();

		m_advSettingsView = Window::GetDefaultWindowView();

		m_backBtn = UI::Button{ "Back", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - yLineSpace);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::StartConfig); });

		// Population Size
		m_popSizeLabel = sf::Text{ "Population size", font };
		m_popSizeLabel.setPosition(50.f, yPos);
		m_popSizeMinusBtn = UI::Button{ "-", m_advSettingsView };
		m_popSizeMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_popSizeMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizeMinusBtn.SetCentreText(true);
		m_popSizeMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) - 1, m_popSizeMinVal, m_popSizeMaxVal))); CheckTFValues(true); });
		m_popSizeTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_advSettingsView };
		m_popSizeTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_popSizeTF.GetLostFocusEvent().AddCallback([&]() { Functions::ClampTextFieldIntegerValue(m_popSizeTF, m_popSizeMinVal, m_popSizeMaxVal); CheckTFValues(true); });
		m_popSizeTF.SetTooltipText("[15-35] Default=25\nThe number of cars in each generation.");
		m_popSizePlusBtn = UI::Button{ "+", m_advSettingsView };
		m_popSizePlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_popSizePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_popSizePlusBtn.SetCentreText(true);
		m_popSizePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_popSizeTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_popSizeTF.GetRawText().toAnsiString()) + 1, m_popSizeMinVal, m_popSizeMaxVal))); CheckTFValues(true); });

		yPos += yGap;

		// Multi Reproduce
		m_onlyPairOnceText.setFont(font);
		m_onlyPairOnceText.setString("Only Pair Once");
		m_onlyPairOnceText.setPosition(50.f, yPos);
		m_onlyPairOnceCB = UI::CheckBox{ m_advSettingsView };
		m_onlyPairOnceCB.setPosition(125.f, yPos + yLineSpace);
		m_onlyPairOnceCB.SetTooltipText("If checked, the reproduction stage will\nonly allow each car to create 1 offspring\neach (two per pair).\nThis will prevent the ability to choose the\na different number of saved and\nkilled cars.");
		m_onlyPairOnceCB.GetCheckBoxUpdateEvent().AddCallback(&AdvancedSettingsMenu::OnPairCBUpdate, *this);

		yPos += yGap;

		// Save Size
		m_saveLabel = sf::Text{ "Save size", font };
		m_saveLabel.setPosition(50.f, yPos);
		m_saveMinusBtn = UI::Button{ "-", m_advSettingsView };
		m_saveMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_saveMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_saveMinusBtn.SetCentreText(true);
		m_saveMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_saveTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_saveTF.GetRawText().toAnsiString()) - 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(true); });
		m_saveTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_advSettingsView };
		m_saveTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_saveTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(true); });
		m_saveTF.SetTooltipText("Sets the number of cars that will\nbe copied to the next generation\nat the end of each generation.");
		m_savePlusBtn = UI::Button{ "+", m_advSettingsView };
		m_savePlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_savePlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_savePlusBtn.SetCentreText(true);
		m_savePlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_saveTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_saveTF.GetRawText().toAnsiString()) + 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(true); });

		yPos += yGap;

		// Kill Size
		m_killLabel = sf::Text{ "Kill size", font };
		m_killLabel.setPosition(50.f, yPos);
		m_killMinusBtn = UI::Button{ "-", m_advSettingsView };
		m_killMinusBtn.setPosition(50.f, yPos + topPadding + yLineSpace);
		m_killMinusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_killMinusBtn.SetCentreText(true);
		m_killMinusBtn.GetMouseClickedEvent().AddCallback([&]() { m_killTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_killTF.GetRawText().toAnsiString()) - 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(false); });
		m_killTF = UI::TextField{ textFieldWidth, UI::TextField::Integer, m_advSettingsView };
		m_killTF.setPosition(50.f + buttonWidth + xGap, yPos + yLineSpace);
		m_killTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(false); });
		m_killTF.SetTooltipText("Sets the number of cars that will\nbe deleted to the next generation\nat the end of each generation.");
		m_killPlusBtn = UI::Button{ "+", m_advSettingsView };
		m_killPlusBtn.setPosition(50.f + buttonWidth + xGap + textFieldWidth + xGap + hzPadding, yPos + topPadding + yLineSpace);
		m_killPlusBtn.SetBackgroundSize(sf::Vector2f{ buttonWidth, buttonWidth });
		m_killPlusBtn.SetCentreText(true);
		m_killPlusBtn.GetMouseClickedEvent().AddCallback([&]() { m_killTF.SetRawText(Functions::DoubleToString(std::clamp(std::stoi(m_killTF.GetRawText().toAnsiString()) + 1, 0, m_popSizeTF.GetIntegerValue()))); CheckTFValues(false); });

		// NN layers
		m_layerView.setViewport(sf::FloatRect{ 0.7f, 0.35f, 0.2f, 0.6f });
		m_layerView.setSize(window.getSize().x * 0.2f, window.getSize().y * 0.6f);
		m_layerView.setCenter(m_layerView.getSize() / 2.f);

		m_layersText.setFont(font);
		m_layersText.setCharacterSize(20);
		m_layersText.setString("Hidden Layers\nThese text fields control the hidden layers\nin the neural network. Adding/removing\na layer will always happen on the last layer.\nThe hidden layers are the layers between\nthe input and output layers in a nn.");
		m_layersText.setPosition(window.getSize().x * 0.5f, 50.f);

		m_layersBackground.setFillColor(sf::Color{ 42, 50, 125 });
		m_layersBackground.setPosition(window.getSize().x * 0.7f, window.getSize().y * 0.35f);
		m_layersBackground.setSize(sf::Vector2f{ window.getSize().x * 0.2f, window.getSize().y * 0.6f });

		UI::TextField* layerTF = new UI::TextField{ 100.f, UI::TextField::Integer, m_layerView };
		layerTF->setPosition(25.f, 25.f);
		layerTF->SetRawText("4");
		m_layerSizes.push_back(layerTF);
		UI::TextField* layer2TF = new UI::TextField{ 100.f, UI::TextField::Integer, m_layerView };
		layer2TF->setPosition(25.f, 25.f + yLineSpace + 15.f);
		layer2TF->SetRawText("3");
		m_layerSizes.push_back(layer2TF);

		InputManager::GetMouseScrolledEvent().AddCallback(&AdvancedSettingsMenu::OnMouseScrolled, *this);

		m_layerSlider = UI::Slider{ sf::Vector2f{window.getSize().x * 0.95f, window.getSize().y * 0.35f}, m_advSettingsView, window.getSize().y * 0.6f };
		m_layerSlider.GetSliderUpdateEvent().AddCallback([&](float val)
		{
			if (m_layerSliderMax > 0)
				m_layerView.setCenter(m_layerView.getSize().x / 2.f, (m_layerView.getSize().y / 2.f) + (m_layerSliderMax * val));
		});

		m_addLayer = UI::Button{ "Add Layer", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_addLayer.setPosition(window.getSize().x * (1.f - 0.35f) - m_addLayer.GetBounds().width + 10.f, window.getSize().y * 0.65f);
		m_addLayer.SetCentreText(true);
		m_addLayer.GetMouseClickedEvent().AddCallback([&]() 
		{
			UI::TextField* layerTF = new UI::TextField{ 100.f, UI::TextField::Integer, m_layerView };
			layerTF->setPosition(25.f, 25.f + ((UI::GetFont().getLineSpacing(30) + 20.f) * m_layerSizes.size()));
			layerTF->SetRawText("4");
			layerTF->SetActive(true);
			m_layerSizes.push_back(layerTF);

			m_layerSliderMax = (25.f + ((m_layerSizes.size()) * (UI::GetFont().getLineSpacing(30) + 20.f))) - m_layerView.getSize().y;
		});

		m_removeLayer = UI::Button{ "Remove Layer", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_removeLayer.setPosition(window.getSize().x * (1.f - 0.35f) - m_removeLayer.GetBounds().width + 10.f, window.getSize().y * 0.65f + (yLineSpace + 15.f));
		m_removeLayer.SetCentreText(true);
		m_removeLayer.GetMouseClickedEvent().AddCallback([&]() 
		{ 
			if (m_layerSizes.size() > 0) 
			{ 
				delete m_layerSizes.back(); 
				m_layerSizes.pop_back();
				m_layerSliderMax = (25.f + ((m_layerSizes.size()) * (UI::GetFont().getLineSpacing(30) + 20.f))) - m_layerView.getSize().y;
			} 
		});

		m_popSizeTF.SetRawText("25");
		m_saveTF.SetRawText("0");
		m_killTF.SetRawText("22");
	}
	
	void AdvancedSettingsMenu::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_advSettingsView);
		window.draw(m_onlyPairOnceText);
		window.draw(m_onlyPairOnceCB);


		window.draw(m_popSizeLabel);
		window.draw(m_popSizePlusBtn);
		window.draw(m_popSizeTF);
		window.draw(m_popSizeMinusBtn);

		window.draw(m_saveLabel);
		window.draw(m_savePlusBtn);
		window.draw(m_saveTF);
		window.draw(m_saveMinusBtn);

		window.draw(m_killLabel);
		window.draw(m_killPlusBtn);
		window.draw(m_killTF);
		window.draw(m_killMinusBtn);

		window.draw(m_layersText);
		window.draw(m_layersBackground);
		window.draw(m_layerSlider);
		window.draw(m_addLayer);
		window.draw(m_removeLayer);

		window.draw(m_backBtn);

		window.setView(m_layerView);
		for (auto tf : m_layerSizes)
		{
			window.draw(*tf);
		}


	}
	
	void AdvancedSettingsMenu::Load()
	{
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
		m_addLayer.SetActive(true);
		m_removeLayer.SetActive(true);
		for (auto tf : m_layerSizes)
		{
			tf->SetActive(true);
		}

		m_backBtn.SetActive(true);

		CheckTFValues(true);
		CheckTFValues(false);
	}
	
	void AdvancedSettingsMenu::Unload()
	{
		std::vector<unsigned int> layerSizes;
		for (auto tf : m_layerSizes)
		{
			unsigned int size = tf->GetUIntegerValue();
			if (size != 0)
				layerSizes.push_back(size);
		}
		layerSizes.push_back(2);
		
		Evolution::EvolutionManager::GetEvolutionManager().SetAdvancedSettings(25, m_saveTF.GetUIntegerValue(), m_killTF.GetUIntegerValue(), !m_onlyPairOnceCB.IsChecked(), layerSizes);

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
		m_addLayer.SetActive(false);
		m_removeLayer.SetActive(false);
		for (auto tf : m_layerSizes)
		{
			tf->SetActive(false);
		}

		m_backBtn.SetActive(false);
	}
}