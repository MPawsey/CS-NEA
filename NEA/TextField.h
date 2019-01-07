#pragma once

#include "UI.h"
#include "Clickable.h"
#include "Logger.h"

namespace UI
{

	class TextField : public Clickable, public sf::Drawable
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

		void OnMouseLeftClick();
		void OnTextEntered(char input);

		virtual void OnMouseClicked();
		virtual void OnDeactivated();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
		const static inline sf::Color UNACTIVE_COLOUR{ 50, 50, 50, 255 };
		const static inline sf::Color ACTIVE_COLOUR{ 40, 46, 71, 255 };

		TextField();
		TextField(const TextField& textField);
		TextField(sf::Vector2f pos, sf::View& view, float width, FieldType type, UI::Padding padding = { 2.f, 2.f, 2.f, 2.f }, unsigned int charSize = 30u);

		TextField& operator=(const TextField& textField);

		const sf::FloatRect& GetBounds() const;

		const sf::String& GetRawText() const;
		void SetRawText(sf::String s);

	};


}