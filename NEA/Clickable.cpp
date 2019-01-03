#include "Clickable.h"
#include "InputManager.h"


namespace UI
{

	Clickable::Clickable(sf::FloatRect bounds, const sf::View& view)
		: m_clickBounds{ bounds }, m_containerView{ view }
	{
		Create();
	}

	Clickable::Clickable(float x, float y, float width, float height, const sf::View& view)
		: m_clickBounds{ x, y, width, height }, m_containerView{ view }
	{
		Create();
	}

	void Clickable::Create()
	{
		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMousePressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Clickable::OnMouseReleased, *this);
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

}
