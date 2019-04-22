#include "Tooltip.h"

#include "InputManager.h"
#include "UI.h"
#include "Window.h"
#include "Colours.h"

namespace UI
{
	// Creates the tooltip
	void UITooltip::Create()
	{
		// Initialises the background of the tooltip
		m_background.setFillColor(Colours::TT_BACK_COLOUR);
		m_background.setOutlineColor(sf::Color::Blue);
		m_background.setOutlineThickness(2.f);

		// Initialises the text of the tooltip
		m_text.setFont(GetFont());
		m_text.setCharacterSize(24);
		m_text.setFillColor(sf::Color::White);
		m_text.setPosition(sf::Vector2f{ 0, 0 });

		// Initialises the mouse moved event if needed
		if (!m_onMouseMoveID.m_valid)
			m_onMouseMoveID = InputManager::GetMouseMovedEvent().AddCallback(&UITooltip::OnMouseMoved, *this);
	}

	// Initialises the tooltip
	UITooltip::UITooltip()
		: defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
	}

	// Initialises the tooltip with text
	UITooltip::UITooltip(std::string content)
		: defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
		SetText(content);
	}

	// Initialises the tooltip with text
	UITooltip::UITooltip(sf::Text content)
		: defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
		SetText(content);
	}

	// Initialises the tooltip from another tooltip
	UITooltip::UITooltip(const UITooltip& tooltip)
	{
		m_background = tooltip.m_background;
		m_text = tooltip.m_text;
		defaultView = tooltip.defaultView;
	}

	// Deconstructor
	// Removes the callback from the mouse moved event
	UITooltip::~UITooltip()
	{
		InputManager::GetMouseMovedEvent().RemoveCallback(m_onMouseMoveID);
	}

	// Copies values from another tooltip to this tooltip
	UITooltip& UITooltip::operator=(const UITooltip& tooltip)
	{
		// Copies the UIElement from the other tooltip to this tooltip
		UIElement::operator=(tooltip);

		m_background = tooltip.m_background;
		m_text = tooltip.m_text;
		defaultView = tooltip.defaultView;

		// Initialises the tooltip event if needed
		if (!m_onMouseMoveID.m_valid)
			m_onMouseMoveID = InputManager::GetMouseMovedEvent().AddCallback(&UITooltip::OnMouseMoved, *this);

		// Returns this so that calls can be chained
		return *this;
	}

	// Updates the tooltip when the mouse is moved
	void UITooltip::OnMouseMoved(sf::Vector2i mousePos)
	{
		// Only works of the tooltip is active
		if (m_isActive)
		{
			// Gets the mouse position in the default window view
			sf::Vector2f transformedPos = InputManager::GetMousePosInView(*defaultView, mousePos);

			// Bounds the tooltip to the window dimensions
			// LEFT-RIGHT
			if (transformedPos.x < 0)
			{
				transformedPos.x = 0;
			}
			else if (transformedPos.x + m_background.getGlobalBounds().width > defaultView->getSize().x)
			{
				transformedPos.x = defaultView->getSize().x - m_background.getGlobalBounds().width;
			}

			// TOP-BOTTOM
			if (transformedPos.y < m_background.getGlobalBounds().height)
			{
				transformedPos.y = m_background.getGlobalBounds().height;
			}
			else if (transformedPos.y > defaultView->getSize().y)
			{
				transformedPos.y = defaultView->getSize().y;
			}

			// Updates the position of the tooltip
			setPosition(transformedPos);
		}
	}

	// Sets whether the tooltip is active or not
	void UITooltip::SetActive(bool isActive)
	{
		// Updates and checks whether the tooltip is active or not
		// Adds/removes the tooltip from the windows late draw list depending on this
		if (m_isActive = isActive)
		{
			Window::AddToLateDraw(*this);
		}
		else
		{
			Window::RemoveFromLateDraw(*this);
		}
	}

	// Sets the text of the tooltip
	void UITooltip::SetText(sf::Text text)
	{
		m_text = text;

		// Updates the background of the tooltip
		m_background.setSize(sf::Vector2f{ m_text.getGlobalBounds().left + m_text.getGlobalBounds().width, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height });
		setOrigin(sf::Vector2f{ 0, m_background.getGlobalBounds().height });
	}

	// Sets the text of the tooltip
	void UITooltip::SetText(std::string text)
	{
		m_text.setString(text);

		// Updates the background of the tooltip
		m_background.setSize(sf::Vector2f{ m_text.getGlobalBounds().width + 5.f, m_text.getGlobalBounds().height + 10.f });
		setOrigin(sf::Vector2f{0, m_background.getGlobalBounds().height});
	}

	// Draws the tooltip to the target
	void UITooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Only works if the tooltip is active
		if (m_isActive)
		{
			// Sets the view to the default window view
			target.setView(*defaultView);

			// Applies the tooltips transform to the target 
			states.transform *= getTransform();

			// Draws the tooltip
			target.draw(m_background, states);
			target.draw(m_text, states);
		}
	}

}