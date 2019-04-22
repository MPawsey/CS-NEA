#pragma once

#include "Slider.h"
#include "Button.h"

namespace Menu
{

	// The states the menu can take
	// Declared as this is called before MenuManager
	enum class MenuState;

	class MapSelectMenu
	{
	private:
		// Views
		sf::View m_mapSelectView, m_mapButtonView;

		// The state to go back to
		MenuState m_prevState;


		// Slider
		UI::UISlider m_slider;
		float m_sliderMax;

		// Buttons
		sf::RectangleShape m_buttonBackground;
		std::vector<UI::UIButton*> m_buttons;
		float m_yGap;

		// Other
		UI::UIButton m_backBtn;
		bool m_isActive = false;

		// Loads the tracks at a given file
		void LoadMap(std::string filename);

		// Loads the buttons for the tracks
		void LoadMenu();

		// Events
		void OnMouseScrolled(int delta);
		void BackPressed();
		
	public:
		// Initialises the map select screen
		void Init();
		// Updates the map select screen (includes drawing to window)
		void Update();
		// Loads the map select screen
		// Requires the state that preceeded this screen
		void Load(MenuState prevState);
		// Unloads the map select screen
		void Unload();
	};

}