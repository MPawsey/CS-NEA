#include "Clickable.h"
#include "InputManager.h"
#include "Window.h"

namespace UI
{
	// A default view for when none is given
	sf::View nullView;

	// Creates a blank or initialised clickable
	UIClickable::UIClickable(bool initialise)
	{
		if (initialise)
			InitialiseEvents();
	}

	// Initialises a clickable from another clickable (copy constructor)
	UIClickable::UIClickable(const UIClickable& clickable)
		: UIElement{ clickable.m_containerView }, m_clickBounds{ clickable.m_clickBounds }
	{
		// Casts this a transformable and copies the transformable settings
		dynamic_cast<sf::Transformable&>(*this) = clickable;

		InitialiseEvents();
	}

	// Initialises a clickable
	UIClickable::UIClickable(sf::FloatRect bounds, sf::View* view)
		: m_clickBounds{ bounds }, UIElement{ view }
	{
		InitialiseEvents();
	}

	// Initialises a clickable
	UIClickable::UIClickable(sf::FloatRect bounds, sf::View& view)
		: m_clickBounds{ bounds }, UIElement{ &view }
	{
		InitialiseEvents();
	}

	// Uninitialises the events when the clickable is deleted
	UIClickable::~UIClickable()
	{
		UninitialiseEvents();
	}

	// Copies values from another clickable to this
	UIClickable& UIClickable::operator=(const UIClickable& clickable)
	{
		// Copies the UIElement to this
		UIElement::operator=(clickable);

		m_containerView = clickable.m_containerView;
		m_clickBounds = clickable.m_clickBounds;

		// Reinitialise events
		// Fixes problems in case of already initialised events
		InitialiseEvents();
		
		// Returns this so calls can be chained
		return *this;
	}
	
	// Initialises the events used in the clickable class
	void UIClickable::InitialiseEvents()
	{
		// Makes sure that the object isn't already initialised
		// Sets the flag to true if not
		if (m_initialised)
			return;
		m_initialised = true;

		// Removes events from the callback list
		InputManager::GetMouseMovedEvent().RemoveCallback(m_mouseMovedID);
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);

		// Pushes new events to the callback list
		m_mouseMovedID = InputManager::GetMouseMovedEvent().AddCallback(&UIClickable::OnMouseMoved, *this);
		m_mousePressedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UIClickable::OnMousePressed, *this);
		m_mouseReleasedID = InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&UIClickable::OnMouseReleased, *this);
	}

	// Updates the clickable when the mouse is moved
	void UIClickable::OnMouseMoved(sf::Vector2i mousePos)
	{
		// Only does something if the checkbox is active or if the button is not pressed
		if (!m_isActive || m_mousePressed)
			return;

		// Variable for storing the position of the cursor if it is in the view
		sf::Vector2f viewPos;

		// Checks whether the cursor is in the view, and whether it is contained by the clickables bounds
		if (InputManager::IsPointInView(*m_containerView, mousePos, viewPos) && getTransform().transformRect(m_clickBounds).contains(viewPos))
		{
			// Sets the mouse hovered flag to true and calls the OnMouseHover subroutine
			m_mouseHovered = true;
			OnMouseHover();
		}
		else if (m_mouseHovered) // Checks to see whether the mouse was hovering the clickable last frame
		{
			// Sets the mouse hovered flag to false and calls the OnMouseUnhover subroutine
			m_mouseHovered = false;
			OnMouseUnhover();
		}
	}

	// Updates the clickable when the mouse is pressed
	void UIClickable::OnMousePressed()
	{
		// Only does something if the checkbox is active
		if (!m_isActive)
			return;

		// Variable for storing the position of the cursor if it is in the view
		sf::Vector2f mousePos;

		// Checks whether the cursor is in the view, and whether it is contained by the clickables bounds
		if (InputManager::IsMouseInView(*m_containerView, mousePos) && getTransform().transformRect(m_clickBounds).contains(mousePos))
		{
			// Sets the mouse pressed flag to true and calls the OnMouseHoverPress subroutine
			m_mousePressed = true;
			OnMouseHoverPress();
		}
	}

	// Updates the clickable when the mouse is released
	void UIClickable::OnMouseReleased()
	{
		// Only does something if the checkbox is active
		if (!m_isActive)
			return;

		// Variable for storing the position of the cursor if it is in the view
		sf::Vector2f mousePos;

		// Checks whether the clickable is pressed, cursor is in the view, and whether it is contained by the clickables bounds
		if (m_mousePressed && InputManager::IsMouseInView(*m_containerView, mousePos) && getTransform().transformRect(m_clickBounds).contains(mousePos))
		{
			// Calls the mouse clicked subroutine
			OnMouseClick();
		}
		else
		{
			// Calls the mouse released subroutine
			OnMouseRelease();
		}

		// Updates the mouse pressed flag
		m_mousePressed = false;
	}

	// Uninitialises the events used by the clickable
	void UIClickable::UninitialiseEvents()
	{
		// Only does something if the events are initialised
		// Sets the flag to false if they are initialised
		if (!m_initialised)
			return;
		m_initialised = false;

		// Removes the callbacks from the event
		InputManager::GetMouseMovedEvent().RemoveCallback(m_mouseMovedID);
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);
	}

	// Sets whether the clickable is active
	void UIClickable::SetActive(bool isActive)
	{
		// Updates and checks whether the clickable is active or not
		// calling the respective subroutine
		if (m_isActive = isActive)
		{
			OnActivated();
		}
		else
		{
			OnDeactivated();
		}
	}

	// Returns the view the clickable is currently in
	const sf::View& UIClickable::GetContainerView() const
	{
		return *m_containerView;
	}
	
	// Returns the click bounds for the clickable
	const sf::FloatRect UIClickable::GetClickBounds() const
	{
		return getTransform().transformRect(m_clickBounds);
	}

}

