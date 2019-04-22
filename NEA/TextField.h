#pragma once

#include "UI.h"
#include "Clickable.h"
#include "Tooltip.h"

namespace UI
{

	class UITextField : public UIClickable
	{
	public:
		// The different types of text fields
		enum FieldType
		{
			None,
			Text,
			Decimal,
			Integer
		};

	private:

		FieldType m_type = FieldType::None;
		bool m_hasFocus = false;
		sf::RectangleShape m_textContainer;
		UI::Padding m_padding;
		sf::Text m_text;
		sf::String m_rawText;
		Event<> m_lostFocusEvent;
		UITooltip m_tooltip;

		// Event ids
		EventID m_leftClickedID, m_textEnteredID;

		// Events
		void OnMouseLeftClick();
		void OnTextEntered(char input);

		// Inherited events
		void OnMouseHover() override;
		void OnMouseUnhover() override;
		void OnMouseHoverPress() override;

		void OnActivated() override;
		void OnDeactivated() override;

		// Draws the text field to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		// Constructors

		// Creates a blank text field (default constructor)
		UITextField() {}
		// Creates and initialises a text field
		UITextField(float width, FieldType type, sf::View& view, UI::Padding padding = { 2.f, 2.f, 2.f, 2.f });
		// Creates and initialises a text field from another text field (copy constructor)
		UITextField(const UITextField& textField);

		// Destructor
		~UITextField();

		// Sets the values of the current text field from another text field
		UITextField& operator=(const UITextField& textField);

		// Returns the event for when the text field loses focus
		Event<>& GetLostFocusEvent();

		// Sets the raw text of the text field
		void SetRawText(sf::String s);

		// Returns the bounds of the text field
		const sf::FloatRect GetBounds() const;

		// Returns the raw text of the text field
		const sf::String& GetRawText() const;

		// Sets the tooltip for the text field
		void SetTooltipText(sf::Text content);
		void SetTooltipText(std::string content);

		// Gets the value of the text field in different types
		const float GetFloatValue();
		const double GetDoubleValue();
		const int GetIntegerValue();
		const unsigned int GetUIntegerValue();

	};


}