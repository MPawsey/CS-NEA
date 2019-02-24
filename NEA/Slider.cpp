#include "Slider.h"
#include "InputManager.h"

namespace UI
{

	Slider::Slider() {}

	Slider::Slider(const Slider& slider)
		: UIElement{ slider.m_containerView }
	{
		m_sliderUpdateEvent = slider.m_sliderUpdateEvent;
		m_sliderVal = m_sliderVal; // Between 0-1
		m_sliderLine = sf::RectangleShape{slider.m_sliderLine};
		m_sliderGrab = sf::CircleShape{slider.m_sliderGrab};
		m_clickBounds = sf::FloatRect{ slider.m_clickBounds };

		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Slider::OnMouseLeftPressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Slider::OnMouseLeftReleased, *this);
		InputManager::GetMouseMovedEvent().AddCallback(&Slider::OnMouseMoved, *this);
	}

	Slider::Slider(sf::Vector2f posTop, sf::View& view, float height, float sliderWidth)
		: m_sliderLine{ sf::Vector2f{ sliderWidth, height } }, m_sliderGrab {sliderWidth * 2.f}, UIElement{ view }
	{
		m_sliderLine.setPosition(posTop);
		m_sliderLine.setFillColor(sf::Color{ 50, 50, 50 });

		m_sliderGrab.setOrigin(sliderWidth * 2.f, sliderWidth * 2.f);
		m_sliderGrab.setPosition(sf::Vector2f{posTop.x + (sliderWidth / 2.f), posTop.y});
		m_sliderGrab.setFillColor(sf::Color{ 4, 46, 71 });

		m_clickBounds = sf::FloatRect{ sf::Vector2f{m_sliderLine.getPosition().x - sliderWidth, m_sliderLine.getPosition().y - sliderWidth}, 
					sf::Vector2f{m_sliderLine.getGlobalBounds().width + (sliderWidth * 2.f), m_sliderLine.getGlobalBounds().height + (sliderWidth * 2.f)} };
	}

	Slider& Slider::operator=(const Slider& slider)
	{
		m_sliderUpdateEvent = slider.m_sliderUpdateEvent;
		m_sliderVal = m_sliderVal; // Between 0-1
		m_sliderLine = sf::RectangleShape{ slider.m_sliderLine };
		m_sliderGrab = sf::CircleShape{ slider.m_sliderGrab };
		m_clickBounds = sf::FloatRect{ slider.m_clickBounds };
		m_containerView = slider.m_containerView;

		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Slider::OnMouseLeftPressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Slider::OnMouseLeftReleased, *this);
		InputManager::GetMouseMovedEvent().AddCallback(&Slider::OnMouseMoved, *this);

		return *this;
	}

	void Slider::CalcSliderVal(sf::Vector2f mousePos)
	{
		m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, std::clamp(mousePos.y, m_sliderLine.getPosition().y, m_sliderLine.getPosition().y + m_sliderLine.getGlobalBounds().height));
		m_sliderVal = (m_sliderGrab.getPosition().y - m_sliderLine.getPosition().y) / m_sliderLine.getGlobalBounds().height;
	}

	void Slider::OnMouseLeftPressed()
	{
		if (!m_active) return;

		sf::Vector2f pos;
		if (InputManager::IsMouseInView(*m_containerView, pos) && m_clickBounds.contains(pos))
		{
			m_hasFocus = true;

			CalcSliderVal(pos);
			m_sliderUpdateEvent.Call(m_sliderVal);
		}
	}

	void Slider::OnMouseLeftReleased()
	{
		if (m_active)
			m_hasFocus = false;
	}

	void Slider::OnMouseMoved(sf::Vector2i mousePos)
	{
		if (!m_active) return;

		if (m_hasFocus)
		{
			CalcSliderVal(InputManager::GetMousePosInView(*m_containerView, mousePos));
			m_sliderUpdateEvent.Call(m_sliderVal);
		}
	}

	float Slider::GetSliderVal()
	{
		return m_sliderVal;
	}

	const Event<float>& Slider::GetSliderUpdateEvent()
	{
		return m_sliderUpdateEvent;
	}

	void Slider::SetActive(bool isActive)
	{
		if (m_active = isActive)
		{
			m_sliderVal = 0;
			m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, m_sliderLine.getPosition().y);
		}
		else
		{
			m_hasFocus = false;
		}
	}

	void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_sliderLine, states);
		target.draw(m_sliderGrab, states);
	}

	void Slider::Move(float percentage)
	{
		m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, std::clamp(m_sliderGrab.getPosition().y + (percentage * m_sliderLine.getGlobalBounds().height), m_sliderLine.getPosition().y, m_sliderLine.getPosition().y + m_sliderLine.getGlobalBounds().height));
		m_sliderVal = (m_sliderGrab.getPosition().y - m_sliderLine.getPosition().y) / m_sliderLine.getGlobalBounds().height;
		m_sliderUpdateEvent.Call(m_sliderVal);
	}

}