#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"
#include "UI.h"

namespace UI
{


	class UITooltip : public UIElement
	{
	private:
		sf::RectangleShape m_background;
		sf::Text m_text;
		bool m_isActive = false;
		const sf::View* defaultView;
		EventID m_onMouseMoveID;

		// Creates the tooltip
		void Create();

		// Draws the tooltip to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:

		// Constructors

		// Creates and initialses a blank tooltip (default constructor)
		UITooltip();
		// Creates and initialises a tooltip
		UITooltip(std::string content);
		UITooltip(sf::Text content);
		// Creates and initialses a tooltip from another tooltip (copy constructor)
		UITooltip(const UITooltip& tooltip);

		// Destructor
		~UITooltip();

		// Sets the values of the current tooltip from another tooltip
		UITooltip& operator=(const UITooltip& tooltip);

		// Events
		void OnMouseMoved(sf::Vector2i mousePos);

		// Sets whether the tooltip is active
		void SetActive(bool isActive) override;

		// Sets the text of the tooltip
		void SetText(sf::Text text);
		void SetText(std::string text);

	};


}