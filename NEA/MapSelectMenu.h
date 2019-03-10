#pragma once

#include "Menu.h"
#include "Slider.h"
#include "Button.h"

namespace Menu
{

	class MapSelectMenu : public Menu
	{
	private:
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

		void OnMouseScrolled(int delta);
		void LoadMap(std::string filename);
		void LoadMenu();

	public:
		void Init();
		void Update();
		void Load();
		void Unload();
	};

}