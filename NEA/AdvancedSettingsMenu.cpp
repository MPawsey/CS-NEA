#include "AdvancedSettingsMenu.h"
#include <sfml/Graphics.hpp>
#include "Window.h"


namespace Menu
{


	// Public

	void AdvancedSettingsMenu::Init()
	{
		m_advSettingsView = Window::GetDefaultWindowView();


		m_onlyPairOnceCB = UI::CheckBox{ m_advSettingsView };
		m_onlyPairOnceCB.setPosition(500.f, 500.f);
		m_onlyPairOnceCB.SetTooltipText("If checked, the reproduction stage will\nonly allow each car to create 1 offspring\neach (two per pair).\nThis will prevent the ability to choose the\nnumber of saved and killed cars.");

		m_onlyPairOnceText.setFont(UI::GetFont());
		m_onlyPairOnceText.setString("Only Pair Once");
	}
	
	void AdvancedSettingsMenu::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_advSettingsView);
		window.draw(m_onlyPairOnceCB);
	}
	
	void AdvancedSettingsMenu::Load()
	{
		m_onlyPairOnceCB.SetActive(true);
	}
	
	void AdvancedSettingsMenu::Unload()
	{
		m_onlyPairOnceCB.SetActive(false);
	}
}