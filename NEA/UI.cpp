#include "UI.h"


namespace UI
{
	// The font used in the program
	sf::Font font;

	// Loads the font from the file
	void Init()
	{
		font.loadFromFile("arial.ttf");
	}

	// Returns a reference to the font instance used in the program
	const sf::Font& GetFont()
	{
		return font;
	}

	// Initialises a blank UI element
	UIElement::UIElement(sf::View& view)
		: m_containerView{ &view }
	{}

	// Initialises a blank UI element
	UIElement::UIElement(sf::View* view)
		: m_containerView{ view }
	{}

	// Copies values from another UI element to this
	UIElement& UIElement::operator=(const UIElement& uiElement)
	{
		// Copies the transformable to this
		Transformable::operator=(uiElement);

		m_isActive = uiElement.m_isActive;
		m_containerView = uiElement.m_containerView;

		// Returns this to that 
		return *this;
	}

	// Returns whether the UI element is active
	const bool UIElement::IsActive() const
	{
		return m_isActive;
	}
}
