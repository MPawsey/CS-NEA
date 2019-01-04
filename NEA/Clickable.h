#pragma once

#include "Logger.h"
#include <sfml/Graphics.hpp>

namespace UI
{

	class Clickable
	{
	private:
		static inline Logger logger{ "Clickable" };

		bool m_mousePressed = false;
		const sf::View& m_containerView;
		sf::FloatRect m_clickBounds;

		// Clickable isn't active by default
		bool m_isActive = false;

		bool m_initialised = false;


		void OnMousePressed();
		void OnMouseReleased();

		virtual void OnActivated();
		virtual void OnDeactivated();
		virtual void OnMouseClicked() = 0;

	protected:

		void UpdateClickBounds(sf::FloatRect bounds);

	public:

		Clickable();
		Clickable(sf::FloatRect bounds, const sf::View& view, bool initialise = false);
		Clickable(float x, float y, float width, float height, const sf::View& view, bool initialise = false);

		void Initialise();

		const sf::View& GetContainerView() const;
		const sf::FloatRect& GetClickBounds() const;

		const bool IsActive() const;
		void SetActive(bool isActive);

	};



}