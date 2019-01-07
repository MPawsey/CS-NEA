#include "Clickable.h"
#include "InputManager.h"
#include "Window.h"

namespace UI
{
	sf::View nullView;

	Clickable::Clickable()
		: m_containerView{ nullView }
	{}

	Clickable::Clickable(sf::FloatRect bounds, sf::View& view, bool initialise)
		: m_clickBounds{ bounds }, m_containerView{ view }
	{
		if (initialise)
			InitialiseClickable();
	}

	Clickable::Clickable(float x, float y, float width, float height, sf::View& view, bool initialise)
		: m_clickBounds{ x, y, width, height }, m_containerView{ view }
	{
		if (initialise)
			InitialiseClickable();
	}

	Clickable::~Clickable()
	{
		InputManager::GetMousePressedEvent(sf::Mouse::Left).RemoveCallback(m_mousePressedID);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).RemoveCallback(m_mouseReleasedID);
	}

	void Clickable::InitialiseClickable()
	{
		if (m_initialised)
			return;
		m_initialised = true;

		m_mousePressedID = InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMousePressed, *this);
		m_mouseReleasedID = InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMouseReleased, *this);
	}

	void Clickable::UpdateView(sf::View& view)
	{
		m_containerView = view;
	}

	void Clickable::UpdateClickBounds(sf::FloatRect bounds)
	{
		m_clickBounds = bounds;
	}

	void Clickable::OnMousePressed()
	{
		if (!m_isActive)
			return;

		sf::Vector2f mousePos;
		if (InputManager::IsMouseInView(m_containerView, mousePos) && m_clickBounds.contains(mousePos))
		{
			m_mousePressed = true;
		}
	}

	void Clickable::OnMouseReleased()
	{
		sf::Vector2f mousePos;
		if (m_mousePressed && InputManager::IsMouseInView(m_containerView, mousePos) && m_clickBounds.contains(mousePos))
		{
			OnMouseClicked();
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

	void Clickable::OnActivated() {}
	void Clickable::OnDeactivated() {}

	sf::View& Clickable::GetContainerView() const
	{
		return m_containerView;
	}

	sf::FloatRect Clickable::GetClickBounds() const
	{
		return m_clickBounds;
	}

}

