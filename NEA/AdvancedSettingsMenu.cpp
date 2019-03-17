#include "AdvancedSettingsMenu.h"
#include <sfml/Graphics.hpp>
#include "Window.h"
#include "MenuManager.h"
#include "EvolutionManager.h"
#include <algorithm>
#include "InputManager.h"

namespace Menu
{


	void AdvancedSettingsMenu::OnPairCBUpdate(bool isChecked)
	{
		unsigned int popSize = Evolution::EvolutionManager::GetEvolutionManager().GetPopulationSize();
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
		unsigned int popSize = Evolution::EvolutionManager::GetEvolutionManager().GetPopulationSize();

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
		float tfWidth = 100.f;

		sf::RenderWindow& window = Window::GetWindow();

		m_advSettingsView = Window::GetDefaultWindowView();

		m_backBtn = UI::Button{ "Back", m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::StartConfig); });

		// Stuff 1
		m_onlyPairOnceCB = UI::CheckBox{ m_advSettingsView };
		m_onlyPairOnceCB.setPosition(100.f, 100.f);
		m_onlyPairOnceCB.SetTooltipText("If checked, the reproduction stage will\nonly allow each car to create 1 offspring\neach (two per pair).\nThis will prevent the ability to choose the\nnumber of saved and killed cars.");
		m_onlyPairOnceCB.GetCheckBoxUpdateEvent().AddCallback(&AdvancedSettingsMenu::OnPairCBUpdate, *this);

		m_onlyPairOnceText.setFont(UI::GetFont());
		m_onlyPairOnceText.setString("Only Pair Once");
		m_onlyPairOnceText.setPosition(100.f, 50.f);

		m_saveTF = UI::TextField{ tfWidth, UI::TextField::Integer, m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_saveTF.setPosition(100.f, 250.f);
		m_saveTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(true); });
		m_saveTF.SetTooltipText("Sets the number of cars that will\nbe copied to the next generation\nat the end of each generation.");

		m_killTF = UI::TextField{ tfWidth, UI::TextField::Integer, m_advSettingsView, { 5.f, 5.f, 0.f, 0.f } };
		m_killTF.setPosition(100.f, 300.f);
		m_killTF.GetLostFocusEvent().AddCallback([&]() { CheckTFValues(false); });
		m_killTF.SetTooltipText("Sets the number of cars that will\nbe deleted to the next generation\nat the end of each generation.");

		// NN layers
		m_layerView.setViewport(sf::FloatRect{ 0.7f, 0.35f, 0.2f, 0.6f });
		m_layerView.setSize(window.getSize().x * 0.2f, window.getSize().y * 0.6f);
		m_layerView.setCenter(m_layerView.getSize() / 2.f);

		m_layersText.setFont(UI::GetFont());
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
		layer2TF->setPosition(25.f, 25.f + UI::GetFont().getLineSpacing(30) + 20.f);
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
		m_removeLayer.setPosition(window.getSize().x * (1.f - 0.35f) - m_removeLayer.GetBounds().width + 10.f, window.getSize().y * 0.65f + (UI::GetFont().getLineSpacing(30) + 20.f));
		m_removeLayer.SetCentreText(true);
		m_removeLayer.GetMouseClickedEvent().AddCallback([&]() 
		{ 
			if (m_layerSizes.size() > 0) 
			{ 
				delete m_layerSizes.back(); 
				m_layerSizes.pop_back(); 
			} 
		});

		m_saveTF.SetRawText("0");
		m_killTF.SetRawText("22");
	}
	
	void AdvancedSettingsMenu::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_advSettingsView);
		window.draw(m_onlyPairOnceText);
		window.draw(m_onlyPairOnceCB);
		window.draw(m_saveTF);
		window.draw(m_killTF);

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
		m_onlyPairOnceCB.SetActive(true);
		m_saveTF.SetActive(true);
		m_killTF.SetActive(true);

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
		
		Evolution::EvolutionManager::GetEvolutionManager().SetAdvancedSettings(m_saveTF.GetUIntegerValue(), m_killTF.GetUIntegerValue(), !m_onlyPairOnceCB.IsChecked(), layerSizes);

		m_onlyPairOnceCB.SetActive(false);
		m_saveTF.SetActive(false);
		m_killTF.SetActive(false);

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