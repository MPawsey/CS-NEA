#include "MapSelectMenu.h"

#include "Window.h"
#include <filesystem>
#include "InputManager.h"
#include "RaceTrack.h"
#include "Simulation.h"

namespace Menu
{

	void MapSelectMenu::OnMouseScrolled(int delta)
	{
		if (m_isActive)
			m_slider.Move(-delta * (yGap / m_sliderMax));
	}

	void MapSelectMenu::LoadMap(std::string filename)
	{
		RaceTrack::LoadFromFile(filename);
		GoToState(MenuState::None); // Unloads map select menu
		Window::GetWindow().setFramerateLimit(Evolution::Simulation::SIMULATION_FRAMERATE);
		Window::SetWindowState(Window::Evolution);
	}

	void MapSelectMenu::LoadMenu()
	{
		// Just don't delete a file and it works for some reason
		m_buttons.clear();

		std::string path = "Cars";

		int pos = 0;
		float startOffset = 10.f;
		yGap = UI::GetFont().getLineSpacing(30) + 20.f;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().extension() != ".cars")
				continue;

			UI::Button b{ entry.path().filename().replace_extension().u8string(), m_mapButtonView, { 5.f } };
			b.setPosition(10.f, startOffset + (pos++ * yGap));
			b.SetBackgroundSize(sf::Vector2f{ m_mapButtonView.getSize().x - 20.f, b.GetClickBounds().height });
			b.GetMouseClickedEvent().AddCallback([=]() { LoadMap(entry.path().u8string()); });
			m_buttons.push_back(b);
		}

		m_sliderMax = (startOffset + ((pos)* yGap)) - m_mapButtonView.getSize().y;

		if (m_sliderMax > 0)
			m_slider.GetSliderUpdateEvent().AddCallback([&](float val) { m_mapButtonView.setCenter(m_mapButtonView.getSize().x / 2.f, (m_mapButtonView.getSize().y / 2.f) + (m_sliderMax * val)); });
	}

	// Public

	void MapSelectMenu::Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_mapSelectView = window.getDefaultView();
		m_mapButtonView = window.getDefaultView();

		m_mapButtonView.setViewport({0.1f, 0.1f, 0.8f, 0.8f});
		m_mapButtonView.setSize(m_mapButtonView.getSize() * 0.8f);
		m_mapButtonView.setCenter(m_mapButtonView.getSize() / 2.f);

		m_buttonBackground.setPosition((sf::Vector2f)window.getSize() * 0.1f);
		m_buttonBackground.setSize(m_mapButtonView.getSize());
		m_buttonBackground.setFillColor(sf::Color{ 42, 50, 125 });

		InputManager::GetMouseScrolledEvent().AddCallback(&MapSelectMenu::OnMouseScrolled, *this);

		m_slider = UI::Slider{ sf::Vector2f{ 775.f, 50.f }, m_mapSelectView, 500.f };

		m_backBtn = UI::Button{ "Back", m_mapSelectView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { GoToState(MenuState::StartConfig); });
	}

	void MapSelectMenu::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_mapSelectView);
		window.draw(m_slider);
		window.draw(m_buttonBackground);
		window.draw(m_backBtn);

		window.setView(m_mapButtonView);
		for (UI::Button& b : m_buttons)
		{
			window.draw(b);
		}
	}

	void MapSelectMenu::Load()
	{
		LoadMenu();

		m_isActive = true;

		m_slider.SetActive(true);
		m_backBtn.SetActive(true);

		for (UI::Button& b : m_buttons)
		{
			b.SetActive(true);
		}
	}

	void MapSelectMenu::Unload()
	{
		m_isActive = false;

		m_slider.SetActive(false);
		m_backBtn.SetActive(false);

		for (UI::Button& b : m_buttons)
		{
			b.SetActive(false);
		}
	}


}