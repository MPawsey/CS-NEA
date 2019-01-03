#pragma once

#include "Clickable.h"
#include "Event.h"
#include <string>
#include "Logger.h"

namespace UI
{

	class Button : public Clickable, public sf::Drawable
	{
	private:
		static inline Logger m_logger{ "Button" };

		Event<> m_mouseClickedEvent;
		sf::Text* m_label;

		sf::FloatRect CreateLabel(sf::Vector2f pos, std::string text, unsigned int characterSize);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

		Button(sf::Vector2f pos, const sf::View& view, std::string text, unsigned int characterSize = 30u);
		~Button();

		virtual void OnMouseClicked();
		Event<>& GetMouseClickedEvent();
		

	};

}