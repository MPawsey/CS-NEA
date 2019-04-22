#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"
#include "UI.h"

namespace UI
{

	class UISlider : public UIElement
	{
	private:

		Event<float> m_sliderUpdateEvent;
		float m_sliderVal = 0; // Between 0-1
		sf::RectangleShape m_sliderLine;
		sf::CircleShape m_sliderGrab;
		bool m_hasFocus = false, m_initialised = false;
		sf::FloatRect m_clickBounds;

		// Calculates the slider position
		void CalcSliderVal(sf::Vector2f mousePos);

		// Events
		void OnMouseLeftPressed();
		void OnMouseLeftReleased();
		void OnMouseMoved(sf::Vector2i mousePos);

		// Draws the slider to the target
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:

		// Constructors
		
		// Creates and initialises a blank slider (default constructor)
		UISlider();
		// Creates and intialises a slider
		UISlider(sf::Vector2f posTop, sf::View& view, float height, float sliderWidth = 5.f);
		// Creates and initialises a slider from another slider (copy constructor)
		UISlider(const UISlider& slider);

		// Sets the values of the current slider from another slider
		UISlider& operator=(const UISlider& slider);

		// Returns the position of the slider
		const float GetSliderVal() const;

		// Returns the event for when the slider is updated
		Event<float>& GetSliderUpdateEvent();

		// Sets whether the slider is active
		void SetActive(bool isActive) override;
		
		// Moves the slider to a specific position
		// percentage must be between [0-1]
		void Move(float percentage);
	};



}