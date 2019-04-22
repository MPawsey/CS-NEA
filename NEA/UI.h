#pragma once

#include <sfml/Graphics/Font.hpp>
#include <sfml/Graphics/View.hpp>
#include <sfml/Graphics/Drawable.hpp>
#include <sfml/Graphics/Transformable.hpp>

namespace UI
{
	// Padding struct to be used by UI
	struct Padding
	{
		float left = 0, right = 0, top = 0, bottom = 0;
	};

	// Initialses any UI features
	void Init();

	// Returns the font used by the UI
	const sf::Font& GetFont();

	class UIElement : public sf::Drawable, public sf::Transformable
	{
	private:

	protected:
		sf::View* m_containerView;
		bool m_isActive = false;

		// Constructors

		// Creates a blank UI element (default constructor)
		UIElement() {}
		// Creates and initialises a UI element
		UIElement(sf::View& view);
		UIElement(sf::View* view);

		// Sets the values of the current UI element from another UI element
		UIElement& operator=(const UIElement& uiElement);


	public:
		// Returns whether the UI element is active
		const bool IsActive() const;

		// Draws the UI element to the target
		// Can be expanded upon when inherited
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}
		
		// Pure virtual function for what to do when the UI element changes active state
		virtual void SetActive(bool isActive) = 0;

	};

}