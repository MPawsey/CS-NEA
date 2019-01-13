#include "Clickable.h"
#include "InputManager.h"
#include "Window.h"

namespace UI
{
	sf::View nullView;

	Clickable::Clickable()
	{
		InitialiseEvents();
	}

	Clickable::Clickable(const Clickable& clickable)
		: m_containerView{ clickable.m_containerView }, m_clickBounds{ clickable.m_clickBounds }
	{
		dynamic_cast<sf::Transformable&>(*this) = clickable;

		InitialiseEvents();
	}

	Clickable::Clickable(sf::FloatRect bounds, sf::View* view)
		: m_clickBounds{ bounds }, m_containerView{ view }
	{
		InitialiseEvents();
	}

	Clickable::Clickable(sf::FloatRect bounds, sf::View& view)
		: m_clickBounds{ bounds }, m_containerView{ &view }
	{
		InitialiseEvents();
	}

	Clickable::~Clickable()
	{
		InputManager::GetMouseMovedEvent().RemoveCallback(m_mouseMovedID);
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);
	}

	Clickable& Clickable::operator=(const Clickable& clickable)
	{
		dynamic_cast<sf::Transformable&>(*this) = clickable;

		m_containerView = clickable.m_containerView;
		m_clickBounds = clickable.m_clickBounds;

		return *this;
	}

	void Clickable::InitialiseEvents()
	{
		m_mouseMovedID = InputManager::GetMouseMovedEvent().AddCallback(&Clickable::OnMouseMoved, *this);
		m_mousePressedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMousePressed, *this);
		m_mouseReleasedID = InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMouseReleased, *this);
	}

	void Clickable::OnMouseMoved(sf::Vector2i mousePos)
	{
		if (!m_isActive)
			return;

		sf::Vector2f viewPos;
		if (InputManager::IsPointInView(*m_containerView, mousePos, viewPos) && getTransform().transformRect(m_clickBounds).contains(viewPos))
		{
			OnMouseHover();
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


	const bool Clickable::IsActive() const
	{
		return m_isActive;
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

