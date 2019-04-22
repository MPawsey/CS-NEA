#include "CarSelectMenu.h"

#include "Window.h"
#include <filesystem>
#include "InputManager.h"
#include "EvolutionManager.h"
#include "MenuManager.h"
#include "Colours.h"

namespace Menu
{
	// Event for when the scroll wheel is used
	void CarSelectMenu::OnMouseScrolled(int delta)
	{
		// Updates the slider if the slider is active and can move
		if (m_isActive && m_sliderMax > 0)
			m_slider.Move(delta * (m_yGap / m_sliderMax));
	}

	// Loads a car generation from a file and changes state to
	// choose the map to simulate the cars on
	void CarSelectMenu::LoadCar(std::string filename)
	{
		Evolution::EvolutionManager::GetEvolutionManager().CreateGenerationFromFile(filename);
		MenuManager::GetMenuManager().GoToState(MenuState::StartMap);
	}

	// Reloads the list of buttons
	void CarSelectMenu::LoadMenu()
	{
		// Deletes the old buttons and clears the buttons list
		for (auto b : m_buttons)
			delete b;
		m_buttons.clear();

		// Creates variables for the car buttons
		std::string path = "Cars";
		int pos = 0;
		float startOffset = 10.f;
		m_yGap = UI::GetFont().getLineSpacing(30) + 20.f;

		// Iterates through each file in the cars directory
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			// Checks to see if the current file is the correct file type
			if (entry.path().extension() != ".cars")
				continue;

			// Creates a button and sets it to load the current file when clicked
			UI::UIButton* b = new UI::UIButton{ entry.path().filename().replace_extension().u8string(), m_carButtonView, { 5.f } };
			b->setPosition(10.f, startOffset + (pos++ * m_yGap));
			b->SetBackgroundSize(sf::Vector2f{ m_carButtonView.getSize().x - 20.f, b->GetClickBounds().height });
			b->GetMouseClickedEvent().AddCallback([=]() { LoadCar(entry.path().u8string()); });

			// Adds the button to the buttons list
			m_buttons.push_back(b);
		}

		// Updates the slider size
		m_sliderMax = (startOffset + ((pos)* m_yGap)) - m_carButtonView.getSize().y;

	}

	// Initialises the car select menu
	void CarSelectMenu::Init()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Initiailises the views for the car select menu
		m_carSelectView = window.getDefaultView();
		m_carButtonView = window.getDefaultView();

		m_carButtonView.setViewport({ 0.1f, 0.1f, 0.8f, 0.8f });
		m_carButtonView.setSize(m_carButtonView.getSize() * 0.8f);
		m_carButtonView.setCenter(m_carButtonView.getSize() / 2.f);

		// Initialises the background for the buttons
		m_buttonBackground.setPosition((sf::Vector2f)window.getSize() * 0.1f);
		m_buttonBackground.setSize(m_carButtonView.getSize());
		m_buttonBackground.setFillColor(Colours::UI_BLUE_BG);

		// Adds a callback for the mouse scroll wheel event
		InputManager::GetMouseScrolledEvent().AddCallback(&CarSelectMenu::OnMouseScrolled, *this);

		// Initialises the slider
		m_slider = UI::UISlider{ sf::Vector2f{ 775.f, 50.f }, m_carSelectView, 500.f };
		m_slider.GetSliderUpdateEvent().AddCallback([&](float val) 
		{ 
			// Scrolls through the list when updated
			if (m_sliderMax > 0)
				m_carButtonView.setCenter(m_carButtonView.getSize().x / 2.f, (m_carButtonView.getSize().y / 2.f) + (m_sliderMax * val)); 
		});

		// Initialises the back button
		m_backBtn = UI::UIButton{ "Back", m_carSelectView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { MenuManager::GetMenuManager().GoToState(MenuState::MainMenu); });
	}

	// Updates the car select menu (includes drawing to window)
	void CarSelectMenu::Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Sets the view to the car select view
		window.setView(m_carSelectView);

		// Draws the UI to the window
		window.draw(m_slider);
		window.draw(m_buttonBackground);
		window.draw(m_backBtn);

		// Sets the window view to the car buttons view and draws the buttons
		window.setView(m_carButtonView);
		for (UI::UIButton* b : m_buttons)
		{
			window.draw(*b);
		}
	}

	// Loads the car select menu
	void CarSelectMenu::Load()
	{
		// Sets the menu activity state to true and loads the menu
		m_isActive = true;
		LoadMenu();

		// Sets the UI elements activity states to true
		m_slider.SetActive(true);
		m_backBtn.SetActive(true);

		for (UI::UIButton* b : m_buttons)
		{
			b->SetActive(true);
		}
	}

	// Unloads the car select menu
	void CarSelectMenu::Unload()
	{
		// Sets the menu activity state to false
		m_isActive = false;

		// Sets the UI elements activity states to false
		m_slider.SetActive(false);
		m_backBtn.SetActive(false);

		for (UI::UIButton* b : m_buttons)
		{
			b->SetActive(false);
		}
	}
}