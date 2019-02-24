#include "CarSelectMenu.h"

#include "Window.h"
#include <filesystem>
#include "Menu.h"
#include "InputManager.h"
#include "EvolutionManager.h"
#include "Slider.h"
#include "Button.h"


namespace Menu::CarSelectMenu
{
	// Private
	sf::View m_carSelectView;
	sf::View m_carButtonView;

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

	void LoadCar(std::string filename)
	{
		Evolution::EvolutionManager::CreateGenerationFromFile(filename);
		Menu::GoToState(Menu::MenuState::StartMap);
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
		m_carSelectView = window.getDefaultView();
		m_carButtonView = window.getDefaultView();

		Window::GetWindowClosedEvent().AddCallback(OnWindowClosed);

		m_carButtonView.setViewport({ 0.1f, 0.1f, 0.8f, 0.8f });
		m_carButtonView.setSize(m_carButtonView.getSize() * 0.8f);
		m_carButtonView.setCenter(m_carButtonView.getSize() / 2.f);

		m_buttonBackground.setPosition((sf::Vector2f)window.getSize() * 0.1f);
		m_buttonBackground.setSize(m_carButtonView.getSize());
		m_buttonBackground.setFillColor(sf::Color{ 42, 50, 125 });

		InputManager::GetMouseScrolledEvent().AddCallback(OnMouseScrolled);

		m_slider = UI::Slider{ sf::Vector2f{ 775.f, 50.f }, m_carSelectView, 500.f };

		std::string path = "Cars";

		int pos = 0;
		float startOffset = 10.f;
		yGap = UI::GetFont().getLineSpacing(30) + 20.f;

		m_backBtn = UI::Button{ "Back", m_carSelectView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { Menu::GoToState(Menu::MenuState::MainMenu); });

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().extension() != ".cars")
				continue;

			UI::Button b{ entry.path().filename().replace_extension().u8string(), m_carButtonView, { 5.f } };
			b.setPosition(10.f, startOffset + (pos++ * yGap));
			b.SetBackgroundSize(sf::Vector2f{ m_carButtonView.getSize().x - 20.f, b.GetClickBounds().height });
			b.GetMouseClickedEvent().AddCallback([=]() { LoadCar(entry.path().u8string()); });
			m_buttons.push_back(b);
		}

		m_sliderMax = (startOffset + ((pos)* yGap)) - m_carButtonView.getSize().y;

		if (m_sliderMax > 0)
			m_slider.GetSliderUpdateEvent().AddCallback([&](float val) { m_carButtonView.setCenter(m_carButtonView.getSize().x / 2.f, (m_carButtonView.getSize().y / 2.f) + (m_sliderMax * val)); });
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_carSelectView);
		window.draw(m_slider);
		window.draw(m_buttonBackground);
		window.draw(m_backBtn);

		window.setView(m_carButtonView);
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