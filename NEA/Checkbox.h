#pragma once
#include "Clickable.h"
#include "Tooltip.h"

namespace UI
{


	class UICheckBox : public UIClickable
	{
	private:

		bool m_isChecked = false;
		sf::RectangleShape m_background;
		sf::Text m_checkedText;
		Event<bool> m_checkBoxUpdateEvent;
		UITooltip m_tooltip;

		// Inherited events
		void OnMouseHover() override;
		void OnMouseUnhover() override;
		void OnMouseClick() override;

		// Draws the checkbox to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:

		// Constructors

		// Creates and initialises a blank checkbox (default constructor)
		UICheckBox();
		// Creates and initialises a checkbox
		UICheckBox(sf::View& view);
		// Creates and initialses a checkbox from another checkbox (copy constructor)
		UICheckBox(const UICheckBox& checkBox);

		// Sets the values of the current checkbox from another checkbox
		UICheckBox& operator=(const UICheckBox& checkBox);

		// Sets the text of the tooltip
		void SetTooltipText(sf::Text content);
		void SetTooltipText(std::string content);

		// Sets whether the checkbox is checked or not
		// Does not call the checked event
		void SetChecked(bool isChecked);

		// Returns whether the checkbox is checked
		const bool IsChecked() const;

		// Returns the event for when the checkbox is updated (through clicking)
		Event<bool>& GetCheckBoxUpdateEvent();
	};




}