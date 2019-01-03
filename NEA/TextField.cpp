#include "TextField.h"
#include "InputManager.h"

namespace UI
{
	enum class TextField::FieldType
	{
		Text,
		Decimal,
		Integer
	};


	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_textContainer, states);
		target.draw(m_text, states);
	}


	TextField::TextField(sf::Vector2f pos, const sf::View& view, sf::Vector2f size, sf::Vector2f padding, unsigned int charSize)
		: Clickable{ sf::FloatRect{pos, size}, view }
	{
		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&TextField::OnMouseLeftClick, *this);

		m_text.setPosition(pos);
		m_text.setFont(GetFont());
		m_text.setCharacterSize(charSize);
		m_text.setFillColor(sf::Color::White);
		
		m_textContainer.setPosition(pos - padding);
		m_textContainer.setSize(size + (padding * 2.f));
		m_textContainer.setFillColor(UNACTIVE_COLOUR);
		m_textContainer.setOutlineColor(sf::Color::Red);
		m_textContainer.setOutlineThickness(1.f);

		OnDeactivated();

	}

	void TextField::OnTextEntered(sf::String input)
	{
		if (m_hasFocus)
		{
			if (m_inputStartPos > InputManager::GetTextEnteredSize())
			{
				m_inputStartPos = InputManager::GetTextEnteredSize();
			}
			else
			{
				m_rawText = input.substring(m_inputStartPos);
				m_text.setString(m_rawText);

				unsigned int pos = 0;
				while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width && pos < m_rawText.getSize())
				{
					m_text.setString(m_rawText.substring(++pos));
				}
			}
		}
	}

	void TextField::OnMouseClicked()
	{
		m_hasFocus = true;
		m_textContainer.setFillColor(ACTIVE_COLOUR);
		m_inputStartPos = InputManager::GetTextEnteredSize();
	}

	void TextField::OnDeactivated()
	{
		m_hasFocus = false;
		m_textContainer.setFillColor(UNACTIVE_COLOUR);
	}


	void TextField::OnMouseLeftClick()
	{
		if (!InputManager::IsMouseInView(GetContainerView()))
			OnDeactivated();
	}
}