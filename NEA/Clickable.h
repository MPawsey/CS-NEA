#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"
#include "UI.h"

namespace UI
{

	class UIClickable : public UIElement
	{
	private:

		bool m_mousePressed = false;
		bool m_mouseHovered = false;

		// EventID's
		EventID m_mouseMovedID;
		EventID m_mousePressedID;
		EventID m_mouseReleasedID;

		// Clickable isn't active by default
		bool m_initialised = false;

		// Initialises/uninitialies events
		void InitialiseEvents();
		void UninitialiseEvents();

		// Events
		void OnMouseMoved(sf::Vector2i mousePos);
		void OnMousePressed();
		void OnMouseReleased();

		// Events to be overridden by inherited members
		virtual void OnMouseHover() {}
		virtual void OnMouseUnhover() {}
		virtual void OnMouseHoverPress() {}
		virtual void OnMouseRelease() {} // Triggered if pressed but not clicked
		virtual void OnMouseClick() {}

		virtual void OnActivated() {}
		virtual void OnDeactivated() {}

	protected:

		// The clickbounds of the clickable
		sf::FloatRect m_clickBounds;

	public:
		// Constructors

		// Creates an uninitialised clickable (default constructor)
		UIClickable(bool initialise = false);
		// Creates and initialises a clickable
		UIClickable(sf::FloatRect bounds, sf::View* view);
		UIClickable(sf::FloatRect bounds, sf::View& view);
		// Creates a clickable from another clickable (copy constructor)
		UIClickable(const UIClickable& clickable);

		// Destructor
		~UIClickable();

		// Sets the values of the current clickable from another clickable
		UIClickable& operator=(const UIClickable& clickable);

		// Returns the clickbounds of the clickable
		const sf::FloatRect GetClickBounds() const;
		// Returns the view containing the clickable
		const sf::View& GetContainerView() const;

		// Sets whether the clickable is active or not
		void SetActive(bool isActive) override;

	};



}