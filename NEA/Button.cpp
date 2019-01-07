#include "Button.h"
#include "UI.h"


namespace UI
{

	sf::FloatRect Button::CreateButton(sf::Vector2f pos, std::string text, Padding padding, unsigned int characterSize)
	{
		m_label = new sf::Text{ text, GetFont(), characterSize};
		m_label->setPosition(pos);

		return sf::FloatRect{ pos - sf::Vector2f{padding.left, padding.top}, 
									sf::Vector2f{ m_label->getLocalBounds().width + padding.left + padding.right, GetFont().getLineSpacing(characterSize) + padding.top + padding.bottom } };
	}

	Button::Button(const Button& btn)
		: Clickable{btn.GetBounds(), btn.GetContainerView(), true}
	{
		delete m_label;

		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = sf::RectangleShape{ btn.m_background };
		m_label = new sf::Text{ *btn.m_label };
		m_textCentred = btn.m_textCentred;
	}

	Button::Button(sf::Vector2f pos, sf::View& view, std::string text, Padding padding, unsigned int characterSize)
		: Clickable{ CreateButton(pos, text, padding, characterSize), view}
	{
		m_background.setPosition(sf::Vector2f{pos.x - padding.left, pos.y - padding.top});
		m_background.setSize(sf::Vector2f{ GetClickBounds().width, GetClickBounds().height });
		m_background.setFillColor(BACK_COLOUR);
		m_background.setOutlineColor(sf::Color::White);
		m_background.setOutlineThickness(1.f);
	}

	Button::~Button()
	{
		delete m_label;
	}

	Button& Button::operator=(const Button& btn)
	{
		delete m_label;

		m_mouseClickedEvent = btn.m_mouseClickedEvent;
		m_padding = btn.m_padding;
		m_background = sf::RectangleShape{ btn.m_background };
		m_label = new sf::Text{ *btn.m_label };
		m_textCentred = btn.m_textCentred;

		UpdateClickBounds(m_background.getGlobalBounds());
		UpdateView(btn.GetContainerView());

		InitialiseClickable();

		return *this;
	}

	void Button::SetBackgroundSize(sf::Vector2f size)
	{
		m_background.setSize(size);

		UpdateClickBounds(sf::FloatRect{m_background.getGlobalBounds()});

		if (m_textCentred)
			SetCentreText(true);
	}
	
	void Button::SetCentreText(bool shouldCentre)
	{
		if (m_textCentred = shouldCentre)
		{
			m_label->setOrigin(sf::Vector2f{ m_label->getLocalBounds().left + (m_label->getLocalBounds().width / 2.f), m_label->getLocalBounds().top + (m_label->getLocalBounds().height / 2.f) });
			m_label->setPosition(sf::Vector2f{m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top} + 
									(sf::Vector2f{m_background.getGlobalBounds().width - m_padding.right, m_background.getGlobalBounds().height - m_padding.bottom} / 2.f));
		}
		else
		{
			m_label->setOrigin(sf::Vector2f{0.f, 0.f});
			m_label->setPosition(sf::Vector2f{m_background.getGlobalBounds().left + m_padding.left, m_background.getGlobalBounds().top + m_padding.top});
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_background, states);
		target.draw(*m_label, states);
	}

	void Button::OnMouseClicked()
	{
		m_mouseClickedEvent.Call();
	}

	const sf::FloatRect& Button::GetBounds() const
	{
		return m_background.getGlobalBounds();
	}

	Event<>& Button::GetMouseClickedEvent()
	{
		return m_mouseClickedEvent;
	}


}