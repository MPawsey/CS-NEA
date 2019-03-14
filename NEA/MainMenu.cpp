#include "MainMenu.h"
#include "Window.h"
#include "MenuManager.h"

namespace Menu
{
	// Public

	void MainMenu::Init()
	{
		sf::RenderWindow& window = Window::GetWindow();
		m_mainMenuView = window.getDefaultView();

		float xPos = 50.f;
		float yPos = 50.f;
		float yGap = UI::GetFont().getLineSpacing(30) + 20.f;
		
		m_startConfigBtn = UI::Button{ "Start with new generation", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_startConfigBtn.setPosition(xPos, yPos);
		m_startConfigBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_startConfigBtn.GetClickBounds().height });
		m_startConfigBtn.GetMouseClickedEvent().AddCallback([]() { MenuManager::GetMenuManager().GoToState(MenuState::StartConfig); });

		yPos += yGap;

		m_startSavedBtn = UI::Button{ "Start with existing generation", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_startSavedBtn.setPosition(xPos, yPos);
		m_startSavedBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_startSavedBtn.GetClickBounds().height });
		m_startSavedBtn.GetMouseClickedEvent().AddCallback([]() { MenuManager::GetMenuManager().GoToState(MenuState::StartCar); });

		yPos += yGap;

		m_mapEditorBtn = UI::Button{ "Launch map editor", m_mainMenuView, { 5.f, 7.f, 2.f, 2.f } };
		m_mapEditorBtn.setPosition(xPos, yPos);
		m_mapEditorBtn.SetBackgroundSize(sf::Vector2f{ 500.f, m_mapEditorBtn.GetClickBounds().height });
		m_mapEditorBtn.GetMouseClickedEvent().AddCallback([]() {  });
	}

	void MainMenu::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.draw(m_startConfigBtn);
		window.draw(m_startSavedBtn);
		window.draw(m_mapEditorBtn);
	}

	void MainMenu::Load()
	{
		Window::GetWindow().setView(m_mainMenuView);

		m_startConfigBtn.SetActive(true);
		m_startSavedBtn.SetActive(true);
		m_mapEditorBtn.SetActive(true);
	}

	void MainMenu::Unload()
	{
		m_startConfigBtn.SetActive(false);
		m_startSavedBtn.SetActive(false);
		m_mapEditorBtn.SetActive(false);
	}


}