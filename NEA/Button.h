#pragma once

#include "Clickable.h"
#include "Event.h"
#include <string>
#include "Logger.h"
#include "UI.h"

namespace UI
{

	class Button : public Clickable, public sf::Drawable
	{
	private:
		static inline const sf::Color BACK_COLOUR{ 40, 46, 71, 255 };

		Event<> m_mouseClickedEvent;
		sf::RectangleShape m_background;
		Padding m_padding;
		sf::Text* m_label;
		bool m_textCentred = false;

		sf::FloatRect CreateButton(sf::Vector2f pos, std::string text, Padding padding, unsigned int characterSize);


		virtual void OnMouseClicked();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:

		Button() {}
		Button(const Button& btn);
		Button(sf::Vector2f pos, const sf::View& view, std::string text, Padding padding = { 0.f, 0.f, 0.f, 0.f }, unsigned int characterSize = 30u);
		~Button();

		void SetBackgroundSize(sf::Vector2f size);
		void SetCentreText(bool shouldCentre);

		const sf::FloatRect& GetBounds() const;

		Event<>& GetMouseClickedEvent();

	};

}