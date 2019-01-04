#include "MainMenu.h"
#include "Button.h"
#include "Window.h"
#include "UI.h"

#include "Menu.h"


namespace Menu::MainMenu
{
	// Private
	sf::View m_mainMenuView;

	/*UI::Button* m_startConfigBtn;
	UI::Button* m_startSavedBtn;
	UI::Button* m_mapEditorBtn;*/

	void StartNewGeneration()
	{
		std::cout << "New Gen Clicked" << std::endl;
		Menu::GoToState(Menu::MenuState::StartConfig);
	}

	void StartExistingGeneration()
	{
		std::cout << "Old Gen Clicked" << std::endl;
	}

	void LaunchMapEditor()
	{
		std::cout << "Map Editor Clicked" << std::endl;
	}

	void OnWindowClosedEvent()
	{
		/*delete m_startConfigBtn;
		delete m_startSavedBtn;
		delete m_mapEditorBtn;*/
	}

	// Public

	void Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_mainMenuView = window.getDefaultView();

		Window::GetWindowClosedEvent().AddCallback(OnWindowClosedEvent);
		
		/*m_startConfigBtn = new UI::Button{ sf::Vector2f{50.f, 1.f * UI::GetFont().getLineSpacing(30)}, m_mainMenuView, "Start with new generation" };
		m_startSavedBtn = new UI::Button{ sf::Vector2f{50.f, 3.f * UI::GetFont().getLineSpacing(30)}, m_mainMenuView, "Start with existing generation" };
		m_mapEditorBtn = new UI::Button{ sf::Vector2f{50.f, 5.f * UI::GetFont().getLineSpacing(30)}, m_mainMenuView, "Launch map editor" };

		m_startConfigBtn->GetMouseClickedEvent().AddCallback(StartNewGeneration);
		m_startSavedBtn->GetMouseClickedEvent().AddCallback(StartExistingGeneration);
		m_mapEditorBtn->GetMouseClickedEvent().AddCallback(LaunchMapEditor);*/
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		/*window.draw(*m_startConfigBtn);
		window.draw(*m_startSavedBtn);
		window.draw(*m_mapEditorBtn);*/
	}

	void Load()
	{
		Window::GetWindow().setView(m_mainMenuView);

		/*m_startConfigBtn->SetActive(true);
		m_startSavedBtn->SetActive(true);
		m_mapEditorBtn->SetActive(true);*/
	}

	void Unload()
	{
		/*m_startConfigBtn->SetActive(false);
		m_startSavedBtn->SetActive(false);
		m_mapEditorBtn->SetActive(false);*/
	}


}