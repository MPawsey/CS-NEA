#pragma once


#include <sfml/Graphics.hpp>

namespace UI
{

	class Clickable
	{
	private:
		bool m_mousePressed = false;
		const sf::View& m_containerView;
		sf::FloatRect m_clickBounds;

		void Create();

		void OnMousePressed();
		void OnMouseReleased();

	public:
		// Clickable isn't active by default
		bool m_isActive = false;

		Clickable(sf::FloatRect bounds, const sf::View& view);
		Clickable(float x, float y, float width, float height, const sf::View& view);

		virtual void OnMouseClicked() = 0;

	};



}