#include "UI.h"


namespace UI
{
	// Private
	sf::Font m_font;



	// Public
	void Init()
	{
		m_font.loadFromFile("arial.ttf");
	}


	const sf::Font& GetFont()
	{
		return m_font;
	}



	UIElement::UIElement(sf::View& view)
		: m_containerView{ &view }
	{}

	UIElement::UIElement(sf::View* view)
		: m_containerView{ view }
	{}


	UIElement& UIElement::operator=(const UIElement& uiElement)
	{
		Transformable::operator=(uiElement);

		m_containerView = uiElement.m_containerView;

		return *this;
	}
}
