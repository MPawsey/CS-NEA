#include "TextField.h"
#include "InputManager.h"
#include "Colours.h"

namespace UI
{
	// Initialises a text field from another text field (copy constructor)
	UITextField::UITextField(const UITextField& textField)
		: UIClickable(textField)
	{
		m_textContainer = textField.m_textContainer;
		m_padding = textField.m_padding;
		m_text = textField.m_text;
		m_rawText = textField.m_rawText;
		m_tooltip = textField.m_tooltip;

		// Initiailises the fields events
		m_leftClickedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UITextField::OnMouseLeftClick, *this);
		m_textEnteredID = InputManager::GetTextEnteredEvent().AddCallback(&UITextField::OnTextEntered, *this);
	}

	// Initialises the text field
	UITextField::UITextField(float width, FieldType type, sf::View& view, UI::Padding padding)
		: m_padding{ padding }, m_type{ type }, UIClickable(sf::FloatRect{ sf::Vector2f{0.f, 0.f}, sf::Vector2f{width + padding.left + padding.right, GetFont().getLineSpacing(30) + padding.top + padding.bottom} }, view)
	{
		// Initiailises the text for the text field
		m_text.setPosition(m_padding.left, m_padding.top);
		m_text.setFont(GetFont());
		m_text.setCharacterSize(30);
		m_text.setFillColor(sf::Color::White);

		// Initiailises the background
		m_textContainer.setSize(sf::Vector2f{ GetClickBounds().width, GetClickBounds().height });
		m_textContainer.setFillColor(Colours::TF_UNACTIVE_COLOUR);
		m_textContainer.setOutlineColor(sf::Color::White);
		m_textContainer.setOutlineThickness(1.f);

		// Initiailises the fields events
		m_leftClickedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UITextField::OnMouseLeftClick, *this);
		m_textEnteredID = InputManager::GetTextEnteredEvent().AddCallback(&UITextField::OnTextEntered, *this);
	}

	// Deconstructor for the text field
	// Removes the callbacks from the events used by the text field
	UITextField::~UITextField()
	{
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_leftClickedID);
		InputManager::GetTextEnteredEvent().RemoveCallback(m_textEnteredID);
	}

	// Copies values from another text field to this
	UITextField& UITextField::operator=(const UITextField& textField)
	{
		// Copies the clickable from the other text field to this one
		UIClickable::operator=(textField);

		m_type = textField.m_type;
		m_textContainer = sf::RectangleShape{ textField.m_textContainer };
		m_padding = textField.m_padding;
		m_text = sf::Text{ textField.m_text };
		m_rawText = sf::String{ textField.m_rawText };
		m_tooltip = textField.m_tooltip;

		// Initialises the events if needed
		if (!m_leftClickedID.m_valid)
			m_leftClickedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UITextField::OnMouseLeftClick, *this);
		if (!m_textEnteredID.m_valid)
			m_textEnteredID = InputManager::GetTextEnteredEvent().AddCallback(&UITextField::OnTextEntered, *this);

		// Returns this so calls can be chained
		return *this;
	}

	// Updates the text field when text is entered
	void UITextField::OnTextEntered(char input)
	{
		// Checks whether the text field has focus
		if (m_hasFocus)
		{
			// Determines what to do depending on the type of text field
			// Note that break is not used outside of if statements as the
			// types allow characters used from some others (below them)
			switch (m_type)
			{
			case Text:
				// Allows alphabetical characters (and spaces)
				if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == ' ')
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
			case Decimal:
				// Allows a decimal point
				if (input == '.' && (m_rawText.find('.') == sf::String::InvalidPos || m_type == Text))
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
			case Integer:
				// Allows the decimal numbers
				if (input >= '0' && input <= '9')
				{
					m_rawText.insert(m_rawText.getSize() - 1, input);
					break;
				}
				
				// Backspace
				if (input == '\b' && m_rawText.getSize() > 1)
				{
					m_rawText.erase(m_rawText.getSize() - 2);
					break;
				}

				// If program gets here, invalid text entered so no more has to be done.
				return;
			}
			// Updates the text displayed and fits it to the text field
			m_text.setString(m_rawText);
			unsigned int pos = 0;
			while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && pos < m_rawText.getSize())
			{
				m_text.setString(m_rawText.substring(++pos));
			}
		}
	}

	// Activates the tooltip when the mouse hovers the field
	void UITextField::OnMouseHover()
	{
		m_tooltip.SetActive(true);
	}

	// Deactivates the tooltip when the mouse no longer hovers the field
	void UITextField::OnMouseUnhover()
	{
		m_tooltip.SetActive(false);
	}

	// Applies focus to the field if not already gained when the mouse is pressed on the field
	void UITextField::OnMouseHoverPress()
	{
		// Only works if the field does not already have focus
		if (!m_hasFocus)
		{
			// Adds an underscore and updates the text displayed to in the field
			// Display is set so that the right side of the text is seen
			m_rawText.insert(m_rawText.getSize(), '_');
			m_text.setString(m_rawText);

			unsigned int pos = 0;
			while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && pos < m_rawText.getSize())
			{
				m_text.setString(m_rawText.substring(++pos));
			}

			// Applies focus and changes the background colour
			m_hasFocus = true;
			m_textContainer.setFillColor(Colours::TF_ACTIVE_COLOUR);

		}
	}

	// Changes the colour of the background when the field is activated
	void UITextField::OnActivated()
	{
		m_textContainer.setFillColor(Colours::TF_UNACTIVE_COLOUR);
	}

	// Updates the text field when it is deactivated
	void UITextField::OnDeactivated()
	{
		// Removes focus form the field
		m_hasFocus = false;

		// Checks whether the button is deactivated or focus lost
		// Changes background depending on this
		if (!IsActive())
			m_textContainer.setFillColor(Colours::TF_DISABLED_COLOUR);
		else
			m_textContainer.setFillColor(Colours::TF_UNACTIVE_COLOUR);

		// Removes the underscore from the text if needed
		if (m_rawText.getSize() > 0 && m_rawText.toAnsiString().back() == '_')
			m_rawText.erase(m_rawText.getSize() - 1);

		// Updates the text displayed in the text field
		m_text.setString(m_rawText);
		unsigned int size = 0;
		while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && size < m_rawText.getSize())
		{
			m_text.setString(m_rawText.substring(0, m_rawText.getSize() - ++size));
		}

		// Calls the lost focus event
		m_lostFocusEvent.Call();
	}

	// Updates the text field when the mouse is pressed
	void UITextField::OnMouseLeftClick()
	{
		// Stores the position of the mouse
		sf::Vector2f pos;

		// Checks whether field has focus and whether the cursor is not the bounds of the text field 
		// Deactivates it if this is true
		if (m_hasFocus && (!InputManager::IsMouseInView(GetContainerView(), pos) || (InputManager::IsMouseInView(GetContainerView(), pos) && !GetClickBounds().contains(pos))))
			OnDeactivated();
	}

	// Draws the text field to the target
	void UITextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Applies the transform of the field to the target
		states.transform *= getTransform();

		// Draws the text field
		target.draw(m_textContainer, states);
		target.draw(m_text, states);
	}

	// Returns the event for when the text field loses focus
	Event<>& UITextField::GetLostFocusEvent()
	{
		return m_lostFocusEvent;
	}

	// Returns the bounds of the text field
	const sf::FloatRect UITextField::GetBounds() const
	{
		return m_textContainer.getGlobalBounds();
	}

	// Returns the raw text of the text field
	const sf::String& UITextField::GetRawText() const
	{
		return m_rawText;
	}

	// Sets the text of the text field
	void UITextField::SetRawText(sf::String s)
	{
		// Clears the text currently in the field
		m_rawText.clear();
		
		// Adds each character one at a time, checking whether the character is valid
		for (char input : s.toAnsiString())
		{
			// Determines what to do depending on the type of text field
			// Note that break is not used outside of if statements as the
			// types allow characters used from some others (below them)
			switch (m_type)
			{
			case Text:
				// Allows alphabetical characters (and spaces)
				if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == ' ')
				{
					m_rawText.insert(m_rawText.getSize(), input);
					break;
				}
			case Decimal:
				// Allows a decimal point
				if (input == '.' && (m_rawText.find('.') == sf::String::InvalidPos || m_type == Text))
				{
					m_rawText.insert(m_rawText.getSize(), input);
					break;
				}
			case Integer:
				// Allows the decimal numbers
				if (input >= '0' && input <= '9')
				{
					m_rawText.insert(m_rawText.getSize(), input);
				}
				break;
			}
		}
		
		// Adds an underscore if the text field currently has focus
		if (m_hasFocus)
			m_rawText.insert(m_rawText.getSize(), '_');
			
		// Updates the text displayed in the text field
		m_text.setString(m_rawText);
		unsigned int pos = 0;
		while (m_text.getGlobalBounds().width > m_textContainer.getGlobalBounds().width - ((m_padding.left + m_padding.right) * 2) && pos < m_rawText.getSize())
		{
			m_text.setString(m_rawText.substring(++pos));
		}

	}

	// Sets the text in the tooltip
	void UITextField::SetTooltipText(sf::Text content)
	{
		m_tooltip.SetText(content);
	}

	// Sets the text in the tooltip
	void UITextField::SetTooltipText(std::string content)
	{
		m_tooltip.SetText(content);
	}

	// Returns the value in the text field as a float
	const float UITextField::GetFloatValue()
	{
		if ((m_type == FieldType::Decimal || m_type == FieldType::Integer) && m_rawText.getSize() == 0)
			SetRawText("0");
		return std::stof(m_rawText.toAnsiString());
	}

	// Returns the value in the text field as a double
	const double UITextField::GetDoubleValue()
	{
		if ((m_type == FieldType::Decimal || m_type == FieldType::Integer) && m_rawText.getSize() == 0)
			SetRawText("0");
		return std::stod(m_rawText.toAnsiString());
	}

	// Returns the value in the text field as an integer
	const int UITextField::GetIntegerValue()
	{
		if ((m_type == FieldType::Decimal || m_type == FieldType::Integer) && m_rawText.getSize() == 0)
			SetRawText("0");
		return std::stoi(m_rawText.toAnsiString());
	}

	// Returns the value in the text field as an unsigned integer
	const unsigned int UITextField::GetUIntegerValue()
	{
		if ((m_type == FieldType::Decimal || m_type == FieldType::Integer) && m_rawText.getSize() == 0)
			SetRawText("0");
		return (unsigned int)(std::stoull(m_rawText.toAnsiString()) % UINT16_MAX);
	}
}
