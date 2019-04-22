#pragma once

#include "Slider.h"
#include "Button.h"

namespace Menu
{
	class CarSelectMenu
	{
	private:
		// Views
		sf::View m_carSelectView, m_carButtonView;

		// Slider
		UI::UISlider m_slider;
		float m_sliderMax;

		// Buttons
		float m_yGap;
		sf::RectangleShape m_buttonBackground;
		std::vector<UI::UIButton*> m_buttons;

		// Other
		UI::UIButton m_backBtn;
		bool m_isActive = false;

		// Loads a car from a given file
		void LoadCar(std::string filename);

		// Loads the buttons for the cars
		void LoadMenu();

		// Events
		void OnMouseScrolled(int delta);

	public:
		// Initialises the car select menu
		void Init();
		// Updates the car select menu (includes drawing to the window)
		void Update();
		// Loads the car select menu
		void Load();
		// Unloads the car select menu
		void Unload();
	};
}