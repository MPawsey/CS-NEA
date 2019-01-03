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
			Text,
			Decimal,
			Integer
		};

	private:
		const FieldType m_type;

		bool m_hasFocus = false;
		sf::RectangleShape m_textContainer;
		sf::Vector2f m_padding;
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

		TextField(sf::Vector2f pos, const sf::View& view, float width, FieldType type, sf::Vector2f padding = sf::Vector2f{ 2.f, 2.f }, unsigned int charSize = 30u);



	};


}