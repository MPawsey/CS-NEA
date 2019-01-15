#pragma once

#include <sfml/Graphics.hpp>
#include "Logger.h"
#include "Event.h"

namespace UI
{

	class Clickable : public sf::Transformable
	{
	private:
		static inline Logger logger{ "Clickable" };

		bool m_mousePressed = false;
		bool m_mouseHovered = false;

		// EventID's
		EventID m_mouseMovedID;
		EventID m_mousePressedID;
		EventID m_mouseReleasedID;

		// Clickable isn't active by default
		bool m_isActive = false;

		void InitialiseEvents();

		void OnMouseMoved(sf::Vector2i mousePos);
		void OnMousePressed();
		void OnMouseReleased();


		virtual void OnMouseHover() {}
		virtual void OnMouseUnhover() {}
		virtual void OnMouseHoverPress() {}
		virtual void OnMouseRelease() {} // Triggered if pressed but not clicked
		virtual void OnMouseClick() {};

		virtual void OnActivated() {}
		virtual void OnDeactivated() {}

	protected:

		sf::View* m_containerView;
		sf::FloatRect m_clickBounds;

	public:

		Clickable();
		Clickable(const Clickable& clickable);
		Clickable(sf::FloatRect bounds, sf::View* view);
		Clickable(sf::FloatRect bounds, sf::View& view);
		~Clickable();

		Clickable& operator=(const Clickable& clickable);

		const bool IsActive() const;
		const sf::FloatRect GetClickBounds() const;
		const sf::View& GetContainerView() const;

		void SetActive(bool isActive);

	};



}