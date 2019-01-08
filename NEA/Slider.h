#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace UI
{

	class Slider : public sf::Drawable
	{
	private:

		sf::View m_containerView;
		Event<float> m_sliderUpdateEvent;
		float m_sliderVal = 0; // Between 0-1
		sf::RectangleShape m_sliderLine;
		sf::CircleShape m_sliderGrab;
		bool m_hasFocus = false;
		bool m_active = false;
		sf::FloatRect m_clickBounds;

		void CalcSliderVal(sf::Vector2f mousePos);

		void OnMouseLeftPressed();
		void OnMouseLeftReleased();
		void OnMouseMoved();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

		Slider();
		Slider(const Slider& slider);
		Slider(sf::Vector2f posTop, sf::View& view, float height, float sliderWidth = 5.f);

		Slider& operator=(const Slider& slider);

		float GetSliderVal();
		const Event<float> GetSliderUpdateEvent();

		void SetActive(bool isActive);
	};



}