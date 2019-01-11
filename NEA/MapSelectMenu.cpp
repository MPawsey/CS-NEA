#include "MapSelectMenu.h"

#include "Window.h"
#include "UI.h"
#include <filesystem>

namespace Menu::MapSelectMenu
{
	// Private
	sf::View m_mapSelectView;
	sf::View m_mapButtonView;

	UI::Slider m_slider;
	float m_sliderMax;

	sf::RectangleShape m_buttonBackground;
	std::vector<UI::Button> m_buttons;

	// Public

	void Init()
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

		m_slider = UI::Slider{ sf::Vector2f{ 775.f, 50.f }, m_mapSelectView, 500.f };

		std::string path = "Tracks";

		float yGap = UI::GetFont().getLineSpacing(30) + 20.f;
		int pos = 0;
		float startOffset = 10.f;

		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			UI::Button b{ sf::Vector2f{ 10.f, startOffset + (pos++ * yGap) }, m_mapButtonView, entry.path().filename().u8string() };
			b.SetBackgroundSize(sf::Vector2f{ m_mapButtonView.getSize().x - 20.f, b.GetClickBounds().height });
			b.GetMouseClickedEvent().AddCallback([=]() { std::cout << "pressed " << entry.path() << "\n"; });
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

		window.setView(m_mapButtonView);
		for (UI::Button& b : m_buttons)
		{
			window.draw(b);
		}
	}

	void Load()
	{
		m_slider.SetActive(true);

		for (UI::Button& b : m_buttons)
		{
			b.SetActive(true);
		}
	}

	void Unload()
	{
		m_slider.SetActive(false);

		for (UI::Button& b : m_buttons)
		{
			b.SetActive(false);
		}
	}


}