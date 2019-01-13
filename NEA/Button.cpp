#include "Button.h"
#include "UI.h"


namespace UI
{

	Button::Button(const Button& btn)
		: Clickable(btn)
	{
		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = btn.m_background;
		m_label = btn.m_label;
		m_textCentred = btn.m_textCentred;
	}

	Button::Button(std::string text, sf::View& view, UI::Padding padding)
		: m_padding{ padding }
	{
		m_label = sf::Text{ text, GetFont(), 30 };

		m_containerView = &view;
		m_clickBounds = sf::FloatRect{ sf::Vector2f{m_padding.left, m_padding.top}, sf::Vector2f{ m_label.getLocalBounds().width + m_padding.left + m_padding.right, GetFont().getLineSpacing(30) + m_padding.top + m_padding.bottom } };

		m_background.setSize(sf::Vector2f{ GetClickBounds().width, GetClickBounds().height });
		m_background.setFillColor(BACK_COLOUR);
		m_background.setOutlineColor(sf::Color::White);
		m_background.setOutlineThickness(1.f);
	}

	Button& Button::operator=(const Button& btn)
	{
		dynamic_cast<Clickable&>(*this) = btn;

		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = btn.m_background;
		m_label = btn.m_label ;
		m_textCentred = btn.m_textCentred;

		return *this;
	}

	void Button::RecalculateButton()
	{
		if (m_textCentred)
		{
			m_label.setPosition(sf::Vector2f{ m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top } +
				(sf::Vector2f{ m_background.getGlobalBounds().width - m_padding.right, m_background.getGlobalBounds().height - m_padding.bottom } / 2.f));
		}
		else
		{
			m_label.setPosition(sf::Vector2f{ m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top });
		}
	}

	void Button::SetBackgroundSize(sf::Vector2f size)
	{
		m_background.setSize(size);

		m_clickBounds = sf::FloatRect{m_background.getGlobalBounds()};

		RecalculateButton();
	}
	
	void Button::SetCentreText(bool shouldCentre)
	{
		if (m_textCentred = shouldCentre)
		{
			m_label.setOrigin(sf::Vector2f{ m_label.getLocalBounds().left + (m_label.getLocalBounds().width / 2.f), m_label.getLocalBounds().top + (m_label.getLocalBounds().height / 2.f) });
			RecalculateButton();
		}
		else
		{
			m_label.setOrigin(sf::Vector2f{0.f, 0.f});
			RecalculateButton();
		}
	}

	void Button::SetPadding(UI::Padding padding)
	{
		m_padding = padding;
		RecalculateButton();
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_background, states);
		target.draw(m_label, states);
	}

	void Button::OnMouseHover()
	{
		m_background.setFillColor(HOVER_COLOUR);
	}

	void Button::OnMouseUnhover()
	{
		m_background.setFillColor(BACK_COLOUR);
	}

	void Button::OnMouseHoverPress()
	{
		m_background.setFillColor(PRESSED_COLOUR);
	}

	void Button::OnMouseRelease() // Triggered if pressed but not clicked
	{
		m_background.setFillColor(BACK_COLOUR);
	}

	void Button::OnMouseClick()
	{
		m_background.setFillColor(BACK_COLOUR);
		m_mouseClickedEvent.Call();
	}

	const sf::FloatRect& Button::GetBounds() const
	{
		return m_background.getGlobalBounds();
	}

	const UI::Padding& Button::GetPadding() const
	{
		return m_padding;
	}

	Event<>& Button::GetMouseClickedEvent()
	{
		return m_mouseClickedEvent;
	}


}