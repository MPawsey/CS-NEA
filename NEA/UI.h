#pragma once

#include <sfml/Graphics/Font.hpp>
#include <sfml/Graphics/View.hpp>
#include <sfml/Graphics/Drawable.hpp>
#include <sfml/Graphics/Transformable.hpp>

namespace UI
{
	struct Padding
	{
		float left = 0, right = 0, top = 0, bottom = 0;
	};

	void Init();

	const sf::Font& GetFont();

	class UIElement : public sf::Drawable, public sf::Transformable
	{
	private:

	protected:
		sf::View* m_containerView;
		bool m_isActive = false;

		UIElement() {}
		UIElement(sf::View& view);
		UIElement(sf::View* view);

		UIElement& operator=(const UIElement& uiElement);


	public:

		const bool IsActive() const;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		virtual void SetActive(bool isActive) = 0;

	};

}