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
		static inline const sf::Color BACK_COLOUR{ 40, 46, 71, 255 };

		Event<> m_mouseClickedEvent;
		sf::RectangleShape m_background;
		sf::Text* m_label;

		sf::FloatRect CreateButton(sf::Vector2f pos, std::string text, sf::Vector2f padding, unsigned int characterSize);


		virtual void OnMouseClicked();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:

		Button(sf::Vector2f pos, const sf::View& view, std::string text, sf::Vector2f padding = sf::Vector2f{ 8.f, 2.f }, unsigned int characterSize = 30u);
		~Button();

		const sf::FloatRect& GetBounds() const;

		Event<>& GetMouseClickedEvent();

	};

}