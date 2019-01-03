#include "TextField.h"
#include "InputManager.h"

namespace UI
{
	


	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_textContainer, states);
		target.draw(m_text, states);
	}


	TextField::TextField(sf::Vector2f pos, const sf::View& view, sf::Vector2f size, FieldType type, sf::Vector2f padding, unsigned int charSize)
		: m_type{ type }, m_padding { padding }, Clickable{ sf::FloatRect{ pos, size }, view }
	{
		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&TextField::OnMouseLeftClick, *this);
		InputManager::GetTextEnteredEvent().AddCallback(&TextField::OnTextEntered, *this);

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

	void TextField::OnTextEntered(char input)
	{
		if (m_hasFocus)
		{
			switch (m_type)
			{
			case Text:
				if ((input >= 'a' && input < 'z') || (input >= 'A' && input <= 'Z') || input == ' ')
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
			case Decimal:
				if (input == '.' && m_rawText.find('.') == sf::String::InvalidPos)
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
			case Integer:
				if (input >= '0' && input <= '9')
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
				
				if (input == '\b' && m_rawText.getSize() > 1)
				{
					m_rawText.erase(m_rawText.getSize() - 2);
				}
				break;
			}

			m_text.setString(m_rawText);

			unsigned int pos = 0;
			while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - (m_padding.x * 2) && pos < m_rawText.getSize())
			{
				m_text.setString(m_rawText.substring(++pos));
			}
		}
	}

	void TextField::OnMouseClicked()
	{
		if (!m_hasFocus)
		{
			m_hasFocus = true;
			m_textContainer.setFillColor(ACTIVE_COLOUR);

			m_rawText.insert(m_rawText.getSize(), '_');
			m_text.setString(m_rawText);
		}
	}

	void TextField::OnDeactivated()
	{
		m_hasFocus = false;
		m_textContainer.setFillColor(UNACTIVE_COLOUR);

		if (m_rawText.getSize() > 0)
			m_rawText.erase(m_rawText.getSize() - 1);
		m_text.setString(m_rawText);
	}


	void TextField::OnMouseLeftClick()
	{
		sf::Vector2f pos;
		if (m_hasFocus && InputManager::IsMouseInView(GetContainerView(), pos) && !GetClickBounds().contains(pos))
			OnDeactivated();
	}
}