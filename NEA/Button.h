#pragma once

#include "UI.h"
#include "Clickable.h"
#include "Event.h"
#include <string>

namespace UI
{

	class UIButton : public UIClickable
	{
	private:
		Event<> m_mouseClickedEvent;
		sf::RectangleShape m_background;
		UI::Padding m_padding;
		sf::Text m_label;
		bool m_textCentred = false;

		// Recalculates the values of the button
		void RecalculateButton();

		// Inherited events
		void OnMouseHover() override;
		void OnMouseUnhover() override;
		void OnMouseHoverPress() override;
		void OnMouseRelease() override; // Triggered if pressed but not clicked
		void OnMouseClick() override;

		void OnActivated() override;
		void OnDeactivated() override;

		// Draws the button to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


	public:

		// Constructors

		// Creates and initialises a blank button (default constructor)
		UIButton() {}
		// Creates and initialises a button
		UIButton(std::string text, sf::View& view, UI::Padding padding = { });
		// Creates and initialises a button from another button (copy constructor)
		UIButton(const UIButton& btn);

		// Sets the values of the current button from another button
		UIButton& operator=(const UIButton& btn);

		// Sets the size of the background of the button
		void SetBackgroundSize(sf::Vector2f size);

		// Sets whether the text should be centred
		void SetCentreText(bool shouldCentre);

		// Sets the padding of the button
		void SetPadding(UI::Padding padding);

		// Returns the bounds of the button
		const sf::FloatRect& GetBounds() const;
		// Returns the padding of the button
		const UI::Padding& GetPadding() const;

		// Returns the event for when the button is pressed
		Event<>& GetMouseClickedEvent();

	};

}