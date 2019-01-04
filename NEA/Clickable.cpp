#include "Clickable.h"
#include "InputManager.h"


namespace UI
{
	Clickable::Clickable()
		: m_clickBounds{ sf::FloatRect{} }, m_containerView{ sf::View{} } 
	{}

	Clickable::Clickable(sf::FloatRect bounds, const sf::View& view, bool initialise)
		: m_clickBounds{ bounds }, m_containerView{ view }
	{
		if (initialise)
			Initialise();
	}

	Clickable::Clickable(float x, float y, float width, float height, const sf::View& view, bool initialise)
		: m_clickBounds{ x, y, width, height }, m_containerView{ view }
	{
		if (initialise)
			Initialise();
	}

	void Clickable::Initialise()
	{
		if (m_initialised)
			return;
		m_initialised = true;

		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMousePressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMouseReleased, *this);
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

	void Clickable::OnMouseReleased()
	{
		sf::Vector2f mousePos;
		if (m_mousePressed && InputManager::IsMouseInView(m_containerView, mousePos) && m_clickBounds.contains(mousePos))
		{
			OnMouseClicked();
		}

		m_mousePressed = false;
	}

	const sf::View& Clickable::GetContainerView() const
	{
		return m_containerView;
	}

	const sf::FloatRect& Clickable::GetClickBounds() const
	{
		return m_clickBounds;
	}

}

