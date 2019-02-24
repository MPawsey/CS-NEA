#pragma once

#include "UI.h"
#include <vector>
#include <sfml/Graphics.hpp>

namespace UI
{

	class Popup : public UIElement
	{
	private:
		std::vector<UIElement*> m_elementList;
		bool m_isVisible = false;
		sf::View m_view;
		sf::RectangleShape m_background;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		

	public:
		
		Popup();
		Popup(sf::View& view);
		Popup(sf::View* view);
		~Popup();

		Popup& operator=(const Popup& popup);

		void AddElement(UIElement* element);
		void RemoveElement(UIElement* element);

		void SetViewportTransform(sf::FloatRect rect);
		void SetViewSize(sf::Vector2f size);

		sf::View& GetView();

		virtual void SetActive(bool isActive);

	};


}