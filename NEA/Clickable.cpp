#include "Clickable.h"
#include "InputManager.h"
#include "Window.h"

namespace UI
{
	sf::View nullView;

	Clickable::Clickable(bool initialise)
	{
		if (initialise)
			InitialiseEvents();
	}

	Clickable::Clickable(const Clickable& clickable)
		: UIElement{ clickable.m_containerView }, m_clickBounds{ clickable.m_clickBounds }
	{
		dynamic_cast<sf::Transformable&>(*this) = clickable;

		InitialiseEvents();
	}

	Clickable::Clickable(sf::FloatRect bounds, sf::View* view)
		: m_clickBounds{ bounds }, UIElement{ view }
	{
		InitialiseEvents();
	}

	Clickable::Clickable(sf::FloatRect bounds, sf::View& view)
		: m_clickBounds{ bounds }, UIElement{ &view }
	{
		InitialiseEvents();
	}

	Clickable::~Clickable()
	{
		UninitialiseEvents();
	}

	Clickable& Clickable::operator=(const Clickable& clickable)
	{
		UIElement::operator=(clickable);

		m_containerView = clickable.m_containerView;
		m_clickBounds = clickable.m_clickBounds;

		// Reinitialise events
		// Fixes problems in case of already initialised events
		InitialiseEvents();

		return *this;
	}

	void Clickable::InitialiseEvents()
	{
		if (m_initialised)
			return;
		m_initialised = true;

		// Removes events from the callback list
		InputManager::GetMouseMovedEvent().RemoveCallback(m_mouseMovedID);
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);

		// Pushes new events to the callback list
		m_mouseMovedID = InputManager::GetMouseMovedEvent().AddCallback(&Clickable::OnMouseMoved, *this);
		m_mousePressedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMousePressed, *this);
		m_mouseReleasedID = InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMouseReleased, *this);
	}

	void Clickable::OnMouseMoved(sf::Vector2i mousePos)
	{
		if (!m_isActive || m_mousePressed)
			return;

		sf::Vector2f viewPos;
		if (InputManager::IsPointInView(*m_containerView, mousePos, viewPos) && getTransform().transformRect(m_clickBounds).contains(viewPos))
		{
			m_mouseHovered = true;
			OnMouseHover();
		}
		else if (m_mouseHovered)
		{
			m_mouseHovered = false;
			OnMouseUnhover();
		}
	}

	void Clickable::OnMousePressed()
	{
		if (!m_isActive)
			return;

		sf::Vector2f mousePos;
		if (InputManager::IsMouseInView(*m_containerView, mousePos) && getTransform().transformRect(m_clickBounds).contains(mousePos))
		{
			m_mousePressed = true;
			OnMouseHoverPress();
		}
	}

	void Clickable::OnMouseReleased()
	{
		if (!m_isActive)
			return;

		sf::Vector2f mousePos;
		if (m_mousePressed && InputManager::IsMouseInView(*m_containerView, mousePos) && getTransform().transformRect(m_clickBounds).contains(mousePos))
		{
			OnMouseClick();
		}
		else
		{
			OnMouseRelease();
		}


		m_mousePressed = false;
	}


	void Clickable::UninitialiseEvents()
	{
		if (!m_initialised)
			return;
		m_initialised = false;

		InputManager::GetMouseMovedEvent().RemoveCallback(m_mouseMovedID);
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);
	}

	void Clickable::SetActive(bool isActive)
	{
		if (m_isActive = isActive)
		{
			OnActivated();
		}
		else
		{
			OnDeactivated();
		}
	}

	const sf::View& Clickable::GetContainerView() const
	{
		return *m_containerView;
	}

	const sf::FloatRect Clickable::GetClickBounds() const
	{
		return getTransform().transformRect(m_clickBounds);
	}

}

