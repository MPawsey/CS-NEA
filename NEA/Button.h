#pragma once

#include "UI.h"
#include "Clickable.h"
#include "Event.h"
#include <string>
#include "Logger.h"

namespace UI
{

	class Button : public Clickable
	{
	private:
		static inline const sf::Color BACK_COLOUR{ 40, 46, 71, 255 };
		static inline const sf::Color HOVER_COLOUR{ 60, 69, 106, 255 };
		static inline const sf::Color PRESSED_COLOUR{ 29, 33, 52, 255 };

		Event<> m_mouseClickedEvent;
		sf::RectangleShape m_background;
		UI::Padding m_padding;
		sf::Text m_label;
		bool m_textCentred = false;


		void RecalculateButton();

		virtual void OnMouseHover();
		virtual void OnMouseUnhover();
		virtual void OnMouseHoverPress();
		virtual void OnMouseRelease(); // Triggered if pressed but not clicked
		virtual void OnMouseClick();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	public:

		Button() {}
		Button(const Button& btn);
		Button(std::string text, sf::View& view, UI::Padding padding = { });

		Button& operator=(const Button& btn);

		
		void SetBackgroundSize(sf::Vector2f size);
		void SetCentreText(bool shouldCentre);
		void SetPadding(UI::Padding padding);


		const sf::FloatRect& GetBounds() const;
		const UI::Padding& GetPadding() const;

		Event<>& GetMouseClickedEvent();

	};

}