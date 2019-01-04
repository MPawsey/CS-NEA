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

		// Clickable isn't active by default
		bool m_isActive = false;

		void Create();

		void OnMousePressed();
		void OnMouseReleased();

		virtual void OnActivated();
		virtual void OnDeactivated();
		virtual void OnMouseClicked() = 0;

	public:

		Clickable(sf::FloatRect bounds, const sf::View& view);
		Clickable(float x, float y, float width, float height, const sf::View& view);

		const sf::View& GetContainerView() const;
		const sf::FloatRect& GetClickBounds() const;

		const bool IsActive() const;
		void SetActive(bool isActive);

	};



}