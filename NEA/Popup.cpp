#include "Popup.h"
#include "Colours.h"

namespace UI
{
	// Initialises a blank popup
	UIPopup::UIPopup()
	{
		m_background.setFillColor(Colours::POP_BACK_COLOUR);
	}

	// Initialises a popup in a view
	UIPopup::UIPopup(sf::View& view)
		: UIElement{ view }
	{
		m_background.setFillColor(Colours::POP_BACK_COLOUR);
	}

	// Initialises a popup in a view
	UIPopup::UIPopup(sf::View* view)
		: UIElement{ view }
	{
		m_background.setFillColor(Colours::POP_BACK_COLOUR);
	}

	// Deconstructor for the popup
	// Delets all elements in the popup
	UIPopup::~UIPopup()
	{
		for (UIElement* element : m_elements)
		{
			delete element;
		}
	}

	// Copies the values from another popup to this popup
	UIPopup& UIPopup::operator=(const UIPopup& popup)
	{
		// Deletes any elements currently in the popup so that it isn't lost
		for (UIElement* element : m_elements)
		{
			delete element;
		}

		m_containerView = popup.m_containerView;
		m_elements = popup.m_elements;
		m_background = popup.m_background;

		// Returns this so that calls can be chained
		return *this;
	}

	// Adds an element to the popup
	void UIPopup::AddElement(UIElement* element)
	{
		m_elements.push_back(element);
	}

	// Removes an element from the popup
	void UIPopup::RemoveElement(UIElement* element)
	{
		// Checks whether the element exists in the popup and removes it
		if (auto e = std::find(m_elements.begin(), m_elements.end(), element); e != m_elements.end())
		{
			m_elements.erase(e);
		}
	}

	// Sets the viewport for the popup
	void UIPopup::SetViewportTransform(sf::FloatRect rect)
	{
		m_view.setViewport(rect);
	}
	
	// Sets the view size for the popup
	void UIPopup::SetViewSize(sf::Vector2f size)
	{
		m_background.setSize(size);
		m_view.setSize(size);
		m_view.setCenter(size / 2.f);
	}

	// Sets whether the popup is active or not
	void UIPopup::SetActive(bool isActive)
	{
		// Updates and checks whether the popup is active
		if (m_isActive = isActive)
		{
			// Activates all of the elements inside of the popup
			for (UIElement* element : m_elements)
			{
				element->SetActive(true);
			}
		}
		else
		{
			// Deactivates all of the elements inside of the popup
			for (UIElement* element : m_elements)
			{
				element->SetActive(false);
			}
		}
	}

	// Returns the view of the popup
	sf::View& UIPopup::GetView()
	{
		return m_view;
	}

	// Draws the popup to the target
	void UIPopup::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Only draws the popup if it is active
		if (m_isActive)
		{
			// Gets the current view of the target and replaces it with the view of the popup
			const sf::View& tmpView = target.getView();
			target.setView(m_view);

			// Draws the background and UI elements
			target.draw(m_background, states);
			for (UIElement* element : m_elements)
			{
				target.draw(*element, states);
			}

			// Resets the view of the target back to its original view
			target.setView(tmpView);
		}
	}
}