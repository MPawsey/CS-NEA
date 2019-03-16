#pragma once

#include "UI.h"
#include "Clickable.h"
#include "Tooltip.h"

namespace UI
{

	class TextField : public Clickable
	{
	public:
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
		Tooltip m_tooltip;

		EventID m_leftClickedID, m_textEnteredID;

		void OnMouseLeftClick();
		void OnTextEntered(char input);

		virtual void OnMouseHover();
		virtual void OnMouseUnhover();
		virtual void OnMouseHoverPress();

		virtual void OnActivated();
		virtual void OnDeactivated();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		const static inline sf::Color DISABLED_COLOUR{ 33, 32, 32, 255 };
		const static inline sf::Color UNACTIVE_COLOUR{ 50, 50, 50, 255 };
		const static inline sf::Color ACTIVE_COLOUR{ 40, 46, 71, 255 };

		TextField() {}
		TextField(const TextField& textField);
		TextField(float width, FieldType type, sf::View& view, UI::Padding padding = { 2.f, 2.f, 2.f, 2.f });
		~TextField();

		TextField& operator=(const TextField& textField);

		Event<>& GetLostFocusEvent();
		sf::FloatRect GetBounds() const;

		const sf::String& GetRawText() const;
		void SetRawText(sf::String s);

		void SetTooltipText(sf::Text content);
		void SetTooltipText(std::string content);

		float GetFloatValue() const;
		double GetDoubleValue() const;
		int GetIntegerValue() const;
		unsigned int GetUIntegerValue() const;

	};


}