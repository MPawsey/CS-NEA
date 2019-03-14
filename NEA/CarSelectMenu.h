#pragma once

#include "Slider.h"
#include "Button.h"

namespace Menu
{
	class CarSelectMenu
	{
	private:
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

		void OnMouseScrolled(int delta);
		void LoadCar(std::string filename);
		void LoadMenu();

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};
}