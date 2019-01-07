#include "TextField.h"
#include "InputManager.h"

namespace UI
{
	




	TextField::TextField() {}

	TextField::TextField(const TextField& textField)
		: Clickable{textField.GetClickBounds(), textField.GetContainerView()}
	{
		m_textContainer = sf::RectangleShape{textField.m_textContainer};
		m_padding = textField.m_padding;
		m_text = sf::Text{textField.m_text};
		m_rawText = sf::String{textField.m_rawText};
	}

	TextField::TextField(sf::Vector2f pos, sf::View& view, float width, FieldType type, Padding padding, unsigned int charSize)
		: m_padding{ padding }, m_type{ type }, Clickable{ sf::FloatRect{ pos - sf::Vector2f{padding.left, padding.top}, 
																				sf::Vector2f{ width, GetFont().getLineSpacing(charSize) } + 
																							  (sf::Vector2f{padding.right, padding.bottom} * 2.f) }, view }
	{
		m_text.setPosition(pos);
		m_text.setFont(GetFont());
		m_text.setCharacterSize(charSize);
		m_text.setFillColor(sf::Color::White);
		
		m_textContainer.setPosition(pos - sf::Vector2f{ padding.left, padding.top });
		m_textContainer.setSize(sf::Vector2f{ GetClickBounds().width, GetClickBounds().height });
		m_textContainer.setFillColor(UNACTIVE_COLOUR);
		m_textContainer.setOutlineColor(sf::Color::White);
		m_textContainer.setOutlineThickness(1.f);

		OnDeactivated();

	}

	TextField& TextField::operator=(const TextField& textField)
	{
		m_type = textField.m_type;
		m_textContainer = sf::RectangleShape{ textField.m_textContainer };
		m_padding = textField.m_padding;
		m_text = sf::Text{ textField.m_text };
		m_rawText = sf::String{ textField.m_rawText };

		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&TextField::OnMouseLeftClick, *this);
		InputManager::GetTextEnteredEvent().AddCallback(&TextField::OnTextEntered, *this);

		UpdateClickBounds(m_textContainer.getGlobalBounds());
		UpdateView(textField.GetContainerView());
		InitialiseClickable();

		return *this;
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

				// If program gets here, invalid text entered so no more has to be done.
				return;
			}

			m_text.setString(m_rawText);

			unsigned int pos = 0;
			while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && pos < m_rawText.getSize())
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
		m_lostFocusEvent.Call();

		m_hasFocus = false;
		m_textContainer.setFillColor(UNACTIVE_COLOUR);

		if (m_rawText.getSize() > 0 && m_rawText.toAnsiString().back() == '_')
			m_rawText.erase(m_rawText.getSize() - 1);
		m_text.setString(m_rawText);
	}


	void TextField::OnMouseLeftClick()
	{
		sf::Vector2f pos;
		if (m_hasFocus && InputManager::IsMouseInView(GetContainerView(), pos) && !GetClickBounds().contains(pos))
			OnDeactivated();
	}

	void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_textContainer, states);
		target.draw(m_text, states);
	}


	const Event<>& TextField::GetLostFocusEvent()
	{
		return m_lostFocusEvent;
	}

	const sf::FloatRect& TextField::GetBounds() const
	{
		return m_textContainer.getGlobalBounds();
	}

	const sf::String& TextField::GetRawText() const
	{
		return m_rawText;
	}

	void TextField::SetRawText(sf::String s)
	{
		m_rawText.clear();
		
		for (char input : s.toAnsiString())
		{
			switch (m_type)
			{
			case Text:
				if ((input >= 'a' && input < 'z') || (input >= 'A' && input <= 'Z') || input == ' ')
				{
					m_rawText.insert(m_rawText.getSize(), input);
					break;
				}
			case Decimal:
				if (input == '.' && m_rawText.find('.') == sf::String::InvalidPos)
				{
					m_rawText.insert(m_rawText.getSize(), input);
					break;
				}
			case Integer:
				if (input >= '0' && input <= '9')
				{
					m_rawText.insert(m_rawText.getSize(), input);
				}
				break;
			}
		}
		
		if (m_hasFocus)
			m_rawText.insert(m_rawText.getSize(), '_');
			
		m_text.setString(m_rawText);


		unsigned int pos = 0;
		while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && pos < m_rawText.getSize())
		{
			m_text.setString(m_rawText.substring(++pos));
		}

	}
}