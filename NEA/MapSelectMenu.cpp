#include "MapSelectMenu.h"

#include "Window.h"
#include "UI.h"

namespace Menu::MapSelectMenu
{
	// Private
	sf::View m_mapSelectView;

	UI::Slider m_slider;


	// Public

	void Init()
	{
		m_mapSelectView = Window::GetWindow().getDefaultView();

		m_slider = UI::Slider{ sf::Vector2f{ 775.f, 50.f }, m_mapSelectView, 500.f };
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_mapSelectView);
		window.draw(m_slider);
	}

	void Load()
	{
		m_slider.SetActive(true);
	}

	void Unload()
	{
		m_slider.SetActive(false);
	}


}