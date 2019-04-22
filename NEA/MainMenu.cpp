#include "MainMenu.h"
#include "Window.h"
#include "MenuManager.h"
#include "MapEditor.h"

namespace Menu
{
	// Initialises the main menu screen
	void MainMenu::Init()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();
		
		// Initialises the main menu view
		m_mainMenuView = window.getDefaultView();

		// Positioning variables for the UI
		float xPos = 50.f;
		float yPos = 50.f;
		float yGap = UI::GetFont().getLineSpacing(30) + 20.f;
		
		// Initialises the start with config button
		m_startConfigBtn = UI::UIButton{ "Start with new generation", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_startConfigBtn.setPosition(xPos, yPos);
		m_startConfigBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_startConfigBtn.GetClickBounds().height });
		m_startConfigBtn.GetMouseClickedEvent().AddCallback([]() { MenuManager::GetMenuManager().GoToState(MenuState::StartConfig); });

		// Increases the y position for the buttons
		yPos += yGap;

		// Initialises the start with file button
		m_startSavedBtn = UI::UIButton{ "Start with existing generation", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_startSavedBtn.setPosition(xPos, yPos);
		m_startSavedBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_startSavedBtn.GetClickBounds().height });
		m_startSavedBtn.GetMouseClickedEvent().AddCallback([]() { MenuManager::GetMenuManager().GoToState(MenuState::StartCar); });

		yPos += yGap;

		// Initialises the map editor button
		m_mapEditorBtn = UI::UIButton{ "Launch map editor", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_mapEditorBtn.setPosition(xPos, yPos);
		m_mapEditorBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_mapEditorBtn.GetClickBounds().height });
		m_mapEditorBtn.GetMouseClickedEvent().AddCallback([]() 
		{ 
			// Changes window state and loads the map editor
			MenuManager::GetMenuManager().GoToState(MenuState::None); 
			Window::SetWindowState(Window::Editor);
			Editor::MapEditor::GetMapEditor().Reset();
			Editor::MapEditor::GetMapEditor().Load();
		});
	}

	// Updates the main menu screen
	void MainMenu::Update()
	{
		// Gets the window instance for the window
		sf::RenderWindow& window = Window::GetWindow();

		// Draws the buttons to the window
		window.draw(m_startConfigBtn);
		window.draw(m_startSavedBtn);
		window.draw(m_mapEditorBtn);
	}
	
	// Loads the main menu screen
	void MainMenu::Load()
	{
		// Sets the view of the window to the main menu view
		Window::GetWindow().setView(m_mainMenuView);

		// Sets the activity state of the buttons to true
		m_startConfigBtn.SetActive(true);
		m_startSavedBtn.SetActive(true);
		m_mapEditorBtn.SetActive(true);
	}

	// Unloads the main menu screen
	void MainMenu::Unload()
	{
		// Sets the activity state of the buttons to false
		m_startConfigBtn.SetActive(false);
		m_startSavedBtn.SetActive(false);
		m_mapEditorBtn.SetActive(false);
	}
}