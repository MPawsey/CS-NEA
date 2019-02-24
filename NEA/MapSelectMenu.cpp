#include "MapSelectMenu.h"

#include "Window.h"
#include <filesystem>
#include "Menu.h"
#include "InputManager.h"
#include "RaceTrack.h"
#include "Simulation.h"
#include "Slider.h"
#include "Button.h"

namespace Menu::MapSelectMenu
{
	// Private
	sf::View m_mapSelectView;
	sf::View m_mapButtonView;

	bool m_isActive = false;

	UI::Slider m_slider;
	float m_sliderMax;
	float yGap;

	UI::Button m_backBtn;

	sf::RectangleShape m_buttonBackground;
	std::vector<UI::Button> m_buttons;

	void OnMouseScrolled(int delta)
	{
		if (m_isActive)
			m_slider.Move(-delta * (yGap / m_sliderMax));
	}

	void LoadMap(std::string filename)
	{
		RaceTrack::LoadFromFile(filename);
		Menu::GoToState(Menu::MenuState::None); // Unloads map select menu
		Window::GetWindow().setFramerateLimit(Evolution::Simulation::SIMULATION_FRAMERATE);
		Window::SetWindowState(Window::Evolution);
	}

	void OnWindowClosed()
	{
		m_backBtn.UninitialiseEvents();
		m_slider.~Slider();

		for (auto& button : m_buttons)
		{
			button.UninitialiseEvents();
		}
	}

	// Public

	void Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_mapSelectView = window.getDefaultView();
		m_mapButtonView = window.getDefaultView();

		Window::GetWindowClosedEvent().AddCallback(OnWindowClosed);

		m_mapButtonView.setViewport({0.1f, 0.1f, 0.8f, 0.8f});
		m_mapButtonView.setSize(m_mapButtonView.getSize() * 0.8f);
		m_mapButtonView.setCenter(m_mapButtonView.getSize() / 2.f);

		m_buttonBackground.setPosition((sf::Vector2f)window.getSize() * 0.1f);
		m_buttonBackground.setSize(m_mapButtonView.getSize());
		m_buttonBackground.setFillColor(sf::Color{ 42, 50, 125 });

		InputManager::GetMouseScrolledEvent().AddCallback(OnMouseScrolled);

		m_slider = UI::Slider{ sf::Vector2f{ 775.f, 50.f }, m_mapSelectView, 500.f };

		std::string path = "Tracks";

		int pos = 0;
		float startOffset = 10.f;
		yGap = UI::GetFont().getLineSpacing(30) + 20.f;

		m_backBtn = UI::Button{ "Back", m_mapSelectView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { Menu::GoToState(Menu::MenuState::StartConfig); });

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().extension() != ".track")
				continue;

			UI::Button b{ entry.path().filename().replace_extension().u8string(), m_mapButtonView, { 5.f } };
			b.setPosition(10.f, startOffset + (pos++ * yGap));
			b.SetBackgroundSize(sf::Vector2f{ m_mapButtonView.getSize().x - 20.f, b.GetClickBounds().height });
			b.GetMouseClickedEvent().AddCallback([=]() { LoadMap(entry.path().u8string()); });
			m_buttons.push_back(b);
		}

		m_sliderMax = (startOffset + ((pos) * yGap)) - m_mapButtonView.getSize().y;

		if (m_sliderMax > 0)
			m_slider.GetSliderUpdateEvent().AddCallback([&](float val) { m_mapButtonView.setCenter(m_mapButtonView.getSize().x / 2.f, (m_mapButtonView.getSize().y / 2.f) + (m_sliderMax * val)); });
	}

	void Update()
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

	void Load()
	{
		m_isActive = true;

		m_slider.SetActive(true);
		m_backBtn.SetActive(true);

		for (UI::Button& b : m_buttons)
		{
			b.SetActive(true);
		}
	}

	void Unload()
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