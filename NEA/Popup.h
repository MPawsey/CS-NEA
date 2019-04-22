#pragma once

#include "UI.h"
#include <vector>
#include <sfml/Graphics.hpp>

namespace UI
{

	class UIPopup : public UIElement
	{
	private:
		std::vector<UIElement*> m_elements;
		sf::View m_view;
		sf::RectangleShape m_background;

		// Draws the popup to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		

	public:
		
		// Constructors

		// Creates and initialises a blank popup (default constructor)
		UIPopup();
		// Creates and initialises a popup
		UIPopup(sf::View& view);
		UIPopup(sf::View* view);
		
		// Destructor
		~UIPopup();

		// Sets the values of the current popup from another popup
		UIPopup& operator=(const UIPopup& popup);

		// Adds an UI element to the popup
		void AddElement(UIElement* element);
		// Removes an UI element from the popup
		void RemoveElement(UIElement* element);

		// Sets the transform of the viewport of the popup
		void SetViewportTransform(sf::FloatRect rect);
		// Sets the size of the view of the popup
		void SetViewSize(sf::Vector2f size);

		// Returns the view of the popup
		// Only required if further details are requied (such as transform)
		sf::View& GetView();

		// Sets whether the popup is active or not
		void SetActive(bool isActive) override;

	};


}