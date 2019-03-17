#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"
#include "UI.h"

namespace UI
{

	class Slider : public UIElement
	{
	private:

		Event<float> m_sliderUpdateEvent;
		float m_sliderVal = 0; // Between 0-1
		sf::RectangleShape m_sliderLine;
		sf::CircleShape m_sliderGrab;
		bool m_hasFocus = false;
		sf::FloatRect m_clickBounds;

		void CalcSliderVal(sf::Vector2f mousePos);

		void OnMouseLeftPressed();
		void OnMouseLeftReleased();
		void OnMouseMoved(sf::Vector2i mousePos);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

		Slider();
		Slider(const Slider& slider);
		Slider(sf::Vector2f posTop, sf::View& view, float height, float sliderWidth = 5.f);

		Slider& operator=(const Slider& slider);

		float GetSliderVal();
		Event<float>& GetSliderUpdateEvent();

		virtual void SetActive(bool isActive);
		void Move(float percentage);
	};



}