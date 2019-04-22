#include "Button.h"
#include "UI.h"
#include "Colours.h"

namespace UI
{

	// Initialises a button from another button (copy constructor)
	UIButton::UIButton(const UIButton& btn)
		: UIClickable(btn)
	{
		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = btn.m_background;
		m_label = btn.m_label;
		m_textCentred = btn.m_textCentred;
	}

	// Initialises a button
	UIButton::UIButton(std::string text, sf::View& view, UI::Padding padding)
		: UIClickable(true), m_padding{ padding }
	{
		m_label = sf::Text{ text, GetFont(), 30 };

		m_containerView = &view;
		// Calculates the click bounds of the button
		m_clickBounds = sf::FloatRect{ sf::Vector2f{m_padding.left, m_padding.top}, sf::Vector2f{ m_label.getLocalBounds().width + m_padding.left + m_padding.right, GetFont().getLineSpacing(30) + m_padding.top + m_padding.bottom } };

		// Initialises the background
		m_background.setSize(sf::Vector2f{ GetClickBounds().width, GetClickBounds().height });
		m_background.setFillColor(Colours::BUTTON_DISABLED_COLOUR);
		m_background.setOutlineColor(sf::Color::White);
		m_background.setOutlineThickness(1.f);
	}

	// Copies values from another button to this button
	UIButton& UIButton::operator=(const UIButton& btn)
	{
		// Copies the UIClickable from the other button
		UIClickable::operator=(btn);

		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = btn.m_background;
		m_label = btn.m_label;
		m_textCentred = btn.m_textCentred;

		// Returns this so that calls can be chained
		return *this;
	}

	// Recalculates the display for the button
	void UIButton::RecalculateButton()
	{
		// Checks whether the text should be centred or not
		if (m_textCentred)
		{
			// Sets the position of the label to the centre of the background
			m_label.setPosition(sf::Vector2f{ m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top } +
				(sf::Vector2f{ m_background.getGlobalBounds().width - m_padding.right, m_background.getGlobalBounds().height - m_padding.bottom } / 2.f));
		}
		else
		{
			// Sets the position of the label to the left of the button
			m_label.setPosition(sf::Vector2f{ m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top });
		}
	}

	// Sets the dimensions of the background
	void UIButton::SetBackgroundSize(sf::Vector2f size)
	{
		m_background.setSize(size);

		// Updates the click bounds and recalculates the button
		m_clickBounds = sf::FloatRect{m_background.getGlobalBounds()};
		RecalculateButton();
	}
	
	// Sets whether the text should be centred in the button
	void UIButton::SetCentreText(bool shouldCentre)
	{
		// Sets and checks whether the button is centred
		if (m_textCentred = shouldCentre)
		{
			// Changes the origin of the label to its centre
			m_label.setOrigin(sf::Vector2f{ m_label.getLocalBounds().left + (m_label.getLocalBounds().width / 2.f), m_label.getLocalBounds().top + (m_label.getLocalBounds().height / 2.f) });
		}
		else
		{
			// Changes the origin of the label to its top left
			m_label.setOrigin(sf::Vector2f{0.f, 0.f});
		}
		// Recalculates the buttons display
		RecalculateButton();
	}

	// Sets the padding for the button and updates its display
	void UIButton::SetPadding(UI::Padding padding)
	{
		m_padding = padding;
		RecalculateButton();
	}

	// Draws the button to the target
	void UIButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Applies the buttons transform to the target
		states.transform *= getTransform();

		// Draws the button
		target.draw(m_background, states);
		target.draw(m_label, states);
	}

	// Changes the background colour when the button is activated
	void UIButton::OnActivated()
	{
		m_background.setFillColor(Colours::BUTTON_BACK_COLOUR);
	}

	// Changes the background colour when the button is deactivated
	void UIButton::OnDeactivated()
	{
		m_background.setFillColor(Colours::BUTTON_DISABLED_COLOUR);
	}

	// Changes the background colour when the cursor is hovering over the button
	void UIButton::OnMouseHover()
	{
		m_background.setFillColor(Colours::BUTTON_HOVER_COLOUR);
	}

	// Changes the background colour when the cursor leaves the bounds of the button
	void UIButton::OnMouseUnhover()
	{
		m_background.setFillColor(Colours::BUTTON_BACK_COLOUR);
	}

	// Changes the background colour when the button is pressed
	void UIButton::OnMouseHoverPress()
	{
		m_background.setFillColor(Colours::BUTTON_PRESSED_COLOUR);
	}

	// Changes the background colour when the button is released without clicking the button
	void UIButton::OnMouseRelease()
	{
		m_background.setFillColor(Colours::BUTTON_BACK_COLOUR);
	}

	// Changes the background colour and calls the mouse clicked event when the button is clicked
	void UIButton::OnMouseClick()
	{
		m_background.setFillColor(Colours::BUTTON_BACK_COLOUR);
		m_mouseClickedEvent.Call();
	}

	// Returns the bounds of the button
	const sf::FloatRect& UIButton::GetBounds() const
	{
		return m_background.getGlobalBounds();
	}

	// Returns the padding of the button
	const UI::Padding& UIButton::GetPadding() const
	{
		return m_padding;
	}

	// Returns the event for when the button is clicked
	Event<>& UIButton::GetMouseClickedEvent()
	{
		return m_mouseClickedEvent;
	}
}