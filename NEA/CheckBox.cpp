#include "CheckBox.h"
#include "Colours.h"

namespace UI
{

	// Initialises a blank checkbox
	UICheckBox::UICheckBox() {}

	// Initialises a checkbox from another checkbox (copy constructor)
	UICheckBox::UICheckBox(const UICheckBox& checkBox)
		: UIClickable{checkBox}
	{
		m_checkBoxUpdateEvent = checkBox.m_checkBoxUpdateEvent;
		m_background = checkBox.m_background;
		m_checkedText = checkBox.m_checkedText;
		m_isChecked = checkBox.m_isChecked;
		m_tooltip = checkBox.m_tooltip;
	}
	
	// Initialises a checkbox
	UICheckBox::UICheckBox(sf::View& view)
		: UIClickable{true}
	{
		m_containerView = &view;

		// Initialises the checkmark
		m_checkedText.setFont(UI::GetFont());
		m_checkedText.setString("X");
		m_checkedText.setOrigin((m_checkedText.getGlobalBounds().width + m_checkedText.getGlobalBounds().left) / 2.f, (m_checkedText.getGlobalBounds().height + m_checkedText.getGlobalBounds().top) / 2.f + 4.f);
		m_checkedText.setPosition(GetFont().getLineSpacing(30) / 2.f, GetFont().getLineSpacing(30) / 2.f);

		// Initiailses the background
		m_background.setSize(sf::Vector2f{ GetFont().getLineSpacing(30), GetFont().getLineSpacing(30) });
		m_background.setFillColor(Colours::CB_BACK_COLOUR);
		m_background.setOutlineColor(sf::Color::White);
		m_background.setOutlineThickness(1.f);

		m_clickBounds = m_background.getGlobalBounds();
	}

	// Copies values from another checkbox to this checkbox
	UICheckBox& UICheckBox::operator=(const UICheckBox& checkBox)
	{
		// Copies the UIClickable from the other checkbox to this checkbox
		UIClickable::operator=(checkBox);

		m_checkBoxUpdateEvent = checkBox.m_checkBoxUpdateEvent;
		m_background = checkBox.m_background;
		m_checkedText = checkBox.m_checkedText;
		m_isChecked = checkBox.m_isChecked;
		m_tooltip = checkBox.m_tooltip;

		// Returns this so that calls can be chained
		return *this;
	}

	// Sets whether the checkbox is checked or not
	// Does not trigger the checkbox update event
	void UICheckBox::SetChecked(bool isChecked)
	{
		m_isChecked = isChecked;
	}

	// Sets the tooltip for the checkbox
	void UICheckBox::SetTooltipText(sf::Text content)
	{
		m_tooltip.SetText(content);
	}

	// Sets the tooltip for the checkbox
	void UICheckBox::SetTooltipText(std::string content)
	{
		m_tooltip.SetText(content);
	}

	// Returns whether the checkbox is checked
	const bool UICheckBox::IsChecked() const
	{
		return m_isChecked;
	}

	// Returns the event for when the checkbox is updated
	Event<bool>& UICheckBox::GetCheckBoxUpdateEvent()
	{
		return m_checkBoxUpdateEvent;
	}

	// Event for when the mouse clicks the checkbox
	void UICheckBox::OnMouseClick()
	{
		m_checkBoxUpdateEvent.Call(m_isChecked = !m_isChecked);
	}

	// Turns on the tooltip when the mouse hovers the checkbox
	void UICheckBox::OnMouseHover()
	{
		m_tooltip.SetActive(true);
	}

	// Turns off the tooltip when the mouse stops hovering the checkbox
	void UICheckBox::OnMouseUnhover()
	{
		m_tooltip.SetActive(false);
	}

	// Draws the checkbox to the target
	void UICheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Applies the checkboxes transform to the target
		states.transform *= getTransform();
		// Draws the checkbox
		target.draw(m_background, states);
		// Only draws the checkmark if the checkbox is checked
		if (m_isChecked)
		{
			target.draw(m_checkedText, states);
		}
	}



}