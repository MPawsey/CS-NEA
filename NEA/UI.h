#pragma once

#include <sfml/Graphics/Font.hpp>
#include <sfml/Graphics/View.hpp>
#include <sfml/Graphics/Drawable.hpp>

namespace UI
{
	struct Padding
	{
		float left = 0, right = 0, top = 0, bottom = 0;
	};

	void Init();

	const sf::Font& GetFont();

	class UIElement : public sf::Drawable
	{
	private:

	protected:
		sf::View* m_containerView;

		UIElement() {}
		UIElement(sf::View& view);
		UIElement(sf::View* view);


	public:

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void SetActive(bool isActive) = 0;

	};

}