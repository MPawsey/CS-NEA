#pragma once

#include <sfml/Graphics.hpp>
#include "Logger.h"
#include "Event.h"

namespace UI
{

	class Clickable
	{
	private:
		static inline Logger logger{ "Clickable" };

		bool m_mousePressed = false;
		sf::View& m_containerView;
		sf::FloatRect m_clickBounds;

		// EventID's
		EventID m_mousePressedID;
		EventID m_mouseReleasedID;

		// Clickable isn't active by default
		bool m_isActive = false;

		bool m_initialised = false;


		void OnMousePressed();
		void OnMouseReleased();

		virtual void OnActivated();
		virtual void OnDeactivated();
		virtual void OnMouseClicked() = 0;

	protected:

		void UpdateView(sf::View& view);
		void UpdateClickBounds(sf::FloatRect bounds);

		void InitialiseClickable();

	public:

		Clickable();
		Clickable(sf::FloatRect bounds, sf::View& view, bool initialise = false);
		Clickable(float x, float y, float width, float height, sf::View& view, bool initialise = false);
		~Clickable();

		sf::View& GetContainerView() const;
		sf::FloatRect GetClickBounds() const;

		const bool IsActive() const;
		void SetActive(bool isActive);

	};



}