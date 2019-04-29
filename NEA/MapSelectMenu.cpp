#include "MapSelectMenu.h"

#include "Window.h"
#include <filesystem>
#include "InputManager.h"
#include "RaceTrack.h"
#include "Simulation.h"
#include "MenuManager.h"
#include "EvolutionManager.h"
#include "Colours.h"

namespace Menu
{
	// Event for when the scroll wheel is used
	void MapSelectMenu::OnMouseScrolled(int delta)
	{
		// Updates the slider if the slider is active and can move
		if (m_isActive && m_sliderMax > 0)
			m_slider.Move(-delta * (m_yGap / m_sliderMax));
	}

	// Loads a track from a file and changes state to the evolution state
	void MapSelectMenu::LoadMap(std::string filename)
	{
		RaceTrack::LoadFromFile(filename);
		MenuManager::GetMenuManager().GoToState(MenuState::None); // Unloads map select menu
		Window::GetWindow().setFramerateLimit(Evolution::Simulation::SIMULATION_FRAMERATE);
		Window::SetWindowState(Window::Evolution);
		Evolution::EvolutionManager::GetEvolutionManager().LoadInitialSimulation();
	}

	// Reloads the list of buttons
	void MapSelectMenu::LoadMenu()
	{
		// Deletes the old buttons and clears the buttons list
		for (auto b : m_buttons)
			delete b;
		m_buttons.clear();

		// Creates variables for the track buttons
		std::string path = "Tracks";
		int pos = 0;
		float startOffset = 10.f;
		m_yGap = UI::GetFont().getLineSpacing(30) + 20.f;

		// Iterates through each file in the tracks directory
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			// Checks to see if the current file is the correct file type
			if (entry.path().extension() != ".track")
				continue;

			// Creates a button and sets it to load the current file when clicked
			UI::UIButton* b = new UI::UIButton{ entry.path().filename().replace_extension().u8string(), m_mapButtonView, { 5.f } };
			b->setPosition(10.f, startOffset + (pos++ * m_yGap));
			b->SetBackgroundSize(sf::Vector2f{ m_mapButtonView.getSize().x - 20.f, b->GetClickBounds().height });
			b->GetMouseClickedEvent().AddCallback([=]() { LoadMap(entry.path().u8string()); });

			// Adds the button to the buttons list
			m_buttons.push_back(b);
		}

		// Updates the slider size
		m_sliderMax = (startOffset + ((pos)* m_yGap)) - m_mapButtonView.getSize().y;

	}

	// Reverts the menu state to the previous state
	void MapSelectMenu::BackPressed()
	{
		Evolution::EvolutionManager::GetEvolutionManager().Reset(true);
		MenuManager::GetMenuManager().GoToState(m_prevState);
	}

	// Initialises the map select menu
	void MapSelectMenu::Init()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Initiailises the views for the track select menu
		m_mapSelectView = window.getDefaultView();
		m_mapButtonView = window.getDefaultView();

		m_mapButtonView.setViewport({0.1f, 0.1f, 0.8f, 0.8f});
		m_mapButtonView.setSize(m_mapButtonView.getSize() * 0.8f);
		m_mapButtonView.setCenter(m_mapButtonView.getSize() / 2.f);

		// Initialises the background for the buttons
		m_buttonBackground.setPosition((sf::Vector2f)window.getSize() * 0.1f);
		m_buttonBackground.setSize(m_mapButtonView.getSize());
		m_buttonBackground.setFillColor(Colours::UI_BLUE_BG);

		// Adds a callback for the mouse scroll wheel event
		InputManager::GetMouseScrolledEvent().AddCallback(&MapSelectMenu::OnMouseScrolled, *this);

		// Initialises the slider
		m_slider = UI::UISlider{ sf::Vector2f{ 775.f, 50.f }, m_mapSelectView, 500.f };
		m_slider.GetSliderUpdateEvent().AddCallback([&](float val)
		{
			// Scrolls through the list when updated
			if (m_sliderMax > 0)
				m_mapButtonView.setCenter(m_mapButtonView.getSize().x / 2.f, (m_mapButtonView.getSize().y / 2.f) + (m_sliderMax * val));
		});

		// Initialises the back button
		m_backBtn = UI::UIButton{ "Back", m_mapSelectView, { 5.f, 5.f, 0.f, 0.f } };
		m_backBtn.setPosition(50.f, window.getSize().y - UI::GetFont().getLineSpacing(30) - 5.f);
		m_backBtn.SetCentreText(true);
		m_backBtn.GetMouseClickedEvent().AddCallback([&]() { this->BackPressed(); });
	}

	// Updates the map select menu (includes drawing to window)
	void MapSelectMenu::Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Sets the view to the track select view
		window.setView(m_mapSelectView);

		// Draws the UI to the window
		window.draw(m_slider);
		window.draw(m_buttonBackground);
		window.draw(m_backBtn);

		// Sets the window view to the map buttons view and draws the buttons
		window.setView(m_mapButtonView);
		for (UI::UIButton* b : m_buttons)
		{
			window.draw(*b);
		}
	}

	// Loads the map select menu
	void MapSelectMenu::Load(MenuState prevState)
	{
		// Sets the menu activity state to true and loads the menu
		m_isActive = true;
		LoadMenu();

		// Sets the previous menu state
		m_prevState = prevState;

		// Sets the UI elements activity states to true
		m_slider.SetActive(true);
		m_backBtn.SetActive(true);

		for (UI::UIButton* b : m_buttons)
		{
			b->SetActive(true);
		}
	}

	// Unloads the map select menu
	void MapSelectMenu::Unload()
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