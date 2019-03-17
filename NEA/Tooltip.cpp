#include "Tooltip.h"

#include "InputManager.h"
#include "UI.h"
#include "Window.h"

namespace UI
{




	void Tooltip::Create()
	{
		m_background.setFillColor(sf::Color{20, 40, 86});
		m_background.setOutlineColor(sf::Color::Blue);
		m_background.setOutlineThickness(2.f);

		m_onMouseMoveID = InputManager::GetMouseMovedEvent().AddCallback(&Tooltip::OnMouseMoved, *this);
	}

	Tooltip::Tooltip()
		: m_defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
	}

	Tooltip::Tooltip(std::string content)
		: m_defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
		SetText(content);
	}

	Tooltip::Tooltip(sf::Text content)
		: m_defaultView{ &Window::GetDefaultWindowView() }
	{
		Create();
		SetText(content);
	}

	Tooltip::Tooltip(const Tooltip& tooltip)
	{
		m_background = tooltip.m_background;
		m_text = tooltip.m_text;
		m_defaultView = tooltip.m_defaultView;
	}

	Tooltip::~Tooltip()
	{
		InputManager::GetMouseMovedEvent().RemoveCallback(m_onMouseMoveID);
	}

	Tooltip& Tooltip::operator=(const Tooltip& tooltip)
	{
		UIElement::operator=(tooltip);

		m_background = tooltip.m_background;
		m_text = tooltip.m_text;
		m_defaultView = tooltip.m_defaultView;


		InputManager::GetMouseMovedEvent().RemoveCallback(m_onMouseMoveID);
		m_onMouseMoveID = InputManager::GetMouseMovedEvent().AddCallback(&Tooltip::OnMouseMoved, *this);

		return *this;
	}

	void Tooltip::OnMouseMoved(sf::Vector2i mousePos)
	{
		if (m_isActive)
		{
			sf::Vector2f transformedPos = InputManager::GetMousePosInView(*m_defaultView, mousePos);

			// Need to bound tooltip to screen
			// LEFT-RIGHT
			if (transformedPos.x < 0)
			{
				transformedPos.x = 0;
			}
			else if (transformedPos.x + m_background.getGlobalBounds().width > m_defaultView->getSize().x)
			{
				transformedPos.x = m_defaultView->getSize().x - m_background.getGlobalBounds().width;
			}

			// TOP-BOTTOM
			if (transformedPos.y < m_background.getGlobalBounds().height)
			{
				transformedPos.y = m_background.getGlobalBounds().height;
			}
			else if (transformedPos.y > m_defaultView->getSize().y)
			{
				transformedPos.y = m_defaultView->getSize().y;
			}

			setPosition(transformedPos);
		}
	}

	void Tooltip::SetActive(bool isActive)
	{
		if (m_isActive = isActive)
		{
			Window::AddToLateDraw(*this);
		}
		else
		{
			Window::RemoveFromLateDraw(*this);
		}
	}

	void Tooltip::SetText(sf::Text text)
	{
		m_text = text;
		m_background.setSize(sf::Vector2f{ m_text.getGlobalBounds().left + m_text.getGlobalBounds().width, m_text.getGlobalBounds().top + m_text.getGlobalBounds().height });
		setOrigin(sf::Vector2f{ 0, m_background.getGlobalBounds().height });
	}
	
	void Tooltip::SetText(std::string text)
	{
		m_text.setFont(GetFont());
		m_text.setCharacterSize(24);
		m_text.setFillColor(sf::Color::White);
		m_text.setPosition(sf::Vector2f{0, 0});
		m_text.setString(text);
		m_background.setSize(sf::Vector2f{ m_text.getGlobalBounds().width + 5.f, m_text.getGlobalBounds().height + 10.f });
		setOrigin(sf::Vector2f{0, m_background.getGlobalBounds().height});
	}


	void Tooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_isActive)
		{
			target.setView(*m_defaultView);

			states.transform *= getTransform();

			target.draw(m_background, states);
			target.draw(m_text, states);
		}
	}

}