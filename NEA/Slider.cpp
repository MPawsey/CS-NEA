#include "Slider.h"
#include "InputManager.h"
#include "Colours.h"

namespace UI
{

	// Initialises a blank slider (default constructor)
	UISlider::UISlider() {}

	// Initialises a slider from another slider (copy constructor)
	UISlider::UISlider(const UISlider& slider)
		: UIElement{ slider.m_containerView }
	{
		m_sliderUpdateEvent = slider.m_sliderUpdateEvent;
		m_sliderVal = m_sliderVal;
		m_sliderLine = sf::RectangleShape{slider.m_sliderLine};
		m_sliderGrab = sf::CircleShape{slider.m_sliderGrab};
		m_clickBounds = sf::FloatRect{ slider.m_clickBounds };

		// Adds the callbacks for events used by the slider
		m_initialised = true;
		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UISlider::OnMouseLeftPressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&UISlider::OnMouseLeftReleased, *this);
		InputManager::GetMouseMovedEvent().AddCallback(&UISlider::OnMouseMoved, *this);
	}

	// Initialises a slider
	UISlider::UISlider(sf::Vector2f posTop, sf::View& view, float height, float sliderWidth)
		: m_sliderLine{ sf::Vector2f{ sliderWidth, height } }, m_sliderGrab {sliderWidth * 2.f}, UIElement{ view }
	{
		// Initialises the slider line
		m_sliderLine.setPosition(posTop);
		m_sliderLine.setFillColor(Colours::SLIDER_LINE_COLOUR);

		// Initiailses the slider grab
		m_sliderGrab.setOrigin(sliderWidth * 2.f, sliderWidth * 2.f);
		m_sliderGrab.setPosition(sf::Vector2f{posTop.x + (sliderWidth / 2.f), posTop.y});
		m_sliderGrab.setFillColor(Colours::SLIDER_GRAB_COLOUR);

		// Sets the clickbounds of the slider
		m_clickBounds = sf::FloatRect{ sf::Vector2f{m_sliderLine.getPosition().x - sliderWidth, m_sliderLine.getPosition().y - sliderWidth}, 
					sf::Vector2f{m_sliderLine.getGlobalBounds().width + (sliderWidth * 2.f), m_sliderLine.getGlobalBounds().height + (sliderWidth * 2.f)} };
	}

	// Copies values from another slider to this slider
	UISlider& UISlider::operator=(const UISlider& slider)
	{
		// Copies the UIElement to this
		UIElement::operator=(slider);

		m_sliderUpdateEvent = slider.m_sliderUpdateEvent;
		m_sliderVal = m_sliderVal;
		m_sliderLine = sf::RectangleShape{ slider.m_sliderLine };
		m_sliderGrab = sf::CircleShape{ slider.m_sliderGrab };
		m_clickBounds = sf::FloatRect{ slider.m_clickBounds };
		m_containerView = slider.m_containerView;

		if (!m_initialised)
		{
			m_initialised = true;
			InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&UISlider::OnMouseLeftPressed, *this);
			InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&UISlider::OnMouseLeftReleased, *this);
			InputManager::GetMouseMovedEvent().AddCallback(&UISlider::OnMouseMoved, *this);
		}

		// Returns this so that calls can be chained
		return *this;
	}

	// Updates the slider position and calculates its value
	void UISlider::CalcSliderVal(sf::Vector2f mousePos)
	{
		m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, std::clamp(mousePos.y, m_sliderLine.getPosition().y, m_sliderLine.getPosition().y + m_sliderLine.getGlobalBounds().height));
		m_sliderVal = (m_sliderGrab.getPosition().y - m_sliderLine.getPosition().y) / m_sliderLine.getGlobalBounds().height;
	}

	// Updates the slider when the left mouse button is pressed
	void UISlider::OnMouseLeftPressed()
	{
		// Only works if the slider is active
		if (!m_isActive) return;

		// Variable to store the position of the cursor
		sf::Vector2f pos;

		// Checks whether the cursor is inside the sliders view, and if it is inside the bounds of the slider
		if (InputManager::IsMouseInView(*m_containerView, pos) && m_clickBounds.contains(pos))
		{
			// Sets the hasFocus flag to true, recalculates the slider value and calls the slider update event
			m_hasFocus = true;
			CalcSliderVal(pos);
			m_sliderUpdateEvent.Call(m_sliderVal);
		}
	}

	// Turns off the has focus flag when the left mouse button is released
	void UISlider::OnMouseLeftReleased()
	{
		// Only works when the cursor is active
		if (m_isActive)
			m_hasFocus = false;
	}

	// Updates the slider when the mouse is moved
	void UISlider::OnMouseMoved(sf::Vector2i mousePos)
	{
		// Only works when the cursor is active
		if (!m_isActive) return;

		// Checks whether the slider has focus
		if (m_hasFocus)
		{
			// Recalculates the slider and calls the slider update event
			CalcSliderVal(InputManager::GetMousePosInView(*m_containerView, mousePos));
			m_sliderUpdateEvent.Call(m_sliderVal);
		}
	}

	// Returns the sliders value
	const float UISlider::GetSliderVal() const
	{
		return m_sliderVal;
	}

	// Returns the slider update event
	Event<float>& UISlider::GetSliderUpdateEvent()
	{
		return m_sliderUpdateEvent;
	}

	// Sets whether the slider is active or not
	void UISlider::SetActive(bool isActive)
	{
		// Updates and checks whether the slider is active or not
		if (m_isActive = isActive)
		{
			// Updates the slider position and value
			m_sliderVal = 0;
			m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, m_sliderLine.getPosition().y);
			m_sliderUpdateEvent.Call(m_sliderVal);
		}
		else
		{
			// Removes the focus from the slider
			m_hasFocus = false;
		}
	}

	// Draws the slider to the target
	void UISlider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Applies the sliders transform to the target and draws the slider
		states.transform *= getTransform();
		target.draw(m_sliderLine, states);
		target.draw(m_sliderGrab, states);
	}

	// Moves the slider grab by a given percentage and updates the slider
	void UISlider::Move(float percentage)
	{
		m_sliderGrab.setPosition(m_sliderGrab.getPosition().x, std::clamp(m_sliderGrab.getPosition().y + (percentage * m_sliderLine.getGlobalBounds().height), m_sliderLine.getPosition().y, m_sliderLine.getPosition().y + m_sliderLine.getGlobalBounds().height));
		m_sliderVal = (m_sliderGrab.getPosition().y - m_sliderLine.getPosition().y) / m_sliderLine.getGlobalBounds().height;
		
		// Calls the slider update event
		m_sliderUpdateEvent.Call(m_sliderVal);
	}

}