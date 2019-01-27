#pragma once

#include <sfml/Graphics.hpp>
#include "Event.h"

namespace UI
{


	class Tooltip : public sf::Drawable, public sf::Transformable
	{
	private:

		sf::RectangleShape m_background;
		sf::Text m_text;
		bool m_isActive = false;
		const sf::View* m_view;
		EventID m_onMouseMoveID;

		void Create();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:


		Tooltip();
		Tooltip(std::string content);
		Tooltip(sf::Text content);
		Tooltip(const Tooltip& tooltip);
		~Tooltip();

		Tooltip& operator=(const Tooltip& tooltip);

		void OnMouseMoved(sf::Vector2i mousePos);

		void SetActive(bool isActive);
		void SetText(sf::Text text);
		void SetText(std::string text);

	};


}