#include "CheckBox.h"

namespace UI
{


	CheckBox::CheckBox()
	{

	}

	CheckBox::CheckBox(const CheckBox& checkBox)
		: Clickable{checkBox}
	{
		m_checkBoxUpdateEvent = checkBox.m_checkBoxUpdateEvent;
		m_background = checkBox.m_background;
		m_checkedText = checkBox.m_checkedText;
		m_isChecked = checkBox.m_isChecked;
		m_tooltip = checkBox.m_tooltip;
	}
	
	CheckBox::CheckBox(sf::View& view)
		: Clickable{true}
	{
		m_containerView = &view;

		m_checkedText.setFont(UI::GetFont());
		m_checkedText.setString("X");
		m_checkedText.setOrigin((m_checkedText.getGlobalBounds().width + m_checkedText.getGlobalBounds().left) / 2.f, (m_checkedText.getGlobalBounds().height + m_checkedText.getGlobalBounds().top) / 2.f + 4.f);
		m_checkedText.setPosition(GetFont().getLineSpacing(30) / 2.f, GetFont().getLineSpacing(30) / 2.f);

		m_background.setSize(sf::Vector2f{ GetFont().getLineSpacing(30), GetFont().getLineSpacing(30) });
		m_background.setFillColor(sf::Color{ 50, 50, 50, 255 });
		m_background.setOutlineColor(sf::Color::White);
		m_background.setOutlineThickness(1.f);

		m_clickBounds = m_background.getGlobalBounds();
	}

	CheckBox& CheckBox::operator=(const CheckBox& checkBox)
	{
		Clickable::operator=(checkBox);

		m_checkBoxUpdateEvent = checkBox.m_checkBoxUpdateEvent;
		m_background = checkBox.m_background;
		m_checkedText = checkBox.m_checkedText;
		m_isChecked = checkBox.m_isChecked;
		m_tooltip = checkBox.m_tooltip;

		return *this;
	}


	void CheckBox::SetTooltipText(sf::Text content)
	{
		m_tooltip.SetText(content);
	}

	void CheckBox::SetTooltipText(std::string content)
	{
		m_tooltip.SetText(content);
	}

	bool CheckBox::IsChecked() const
	{
		return m_isChecked;
	}

	Event<bool>& CheckBox::GetCheckBoxUpdateEvent()
	{
		return m_checkBoxUpdateEvent;
	}

	void CheckBox::OnMouseClick()
	{
		m_checkBoxUpdateEvent.Call(m_isChecked = !m_isChecked);
	}

	void CheckBox::OnMouseHover()
	{
		m_tooltip.SetActive(true);
	}

	void CheckBox::OnMouseUnhover()
	{
		m_tooltip.SetActive(false);
	}

	void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_background, states);
		if (m_isChecked)
		{
			target.draw(m_checkedText, states);
		}
	}



}