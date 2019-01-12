#pragma once

#include "UI.h"
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
		UI::Padding m_padding;
		sf::Text m_label;
		bool m_textCentred = false;

		//virtual void OnMouseHover();
		//virtual void OnMouseHoverPress();
		//virtual void OnMouseRelease(); // Triggered if pressed but not clicked
		virtual void OnMouseClick();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:

		Button() {}
		Button(const Button& btn);
		Button(sf::Vector2f pos, sf::View& view, std::string text, UI::Padding padding = { 0.f, 0.f, 0.f, 0.f }, unsigned int characterSize = 30u);

		Button& operator=(const Button& btn);

		void SetBackgroundSize(sf::Vector2f size);
		void SetCentreText(bool shouldCentre);

		const sf::FloatRect& GetBounds() const;

		Event<>& GetMouseClickedEvent();

	};

}