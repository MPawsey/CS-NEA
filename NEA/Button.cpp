#include "Button.h"
#include "UI.h"


namespace UI
{

	sf::FloatRect Button::CreateLabel(sf::Vector2f pos, std::string text, unsigned int characterSize)
	{
		m_label = new sf::Text{ text, GetFont(), characterSize};
		m_label->setPosition(pos);
		return m_label->getGlobalBounds();
	}

	Button::Button(sf::Vector2f pos, const sf::View& view, std::string text, unsigned int characterSize)
		: Clickable{CreateLabel(pos, text, characterSize), view}
	{

	}

	Button::~Button()
	{
		delete m_label;
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(*m_label, states);
	}

	void Button::OnMouseClicked()
	{
		m_mouseClickedEvent.Call();
	}

	Event<>& Button::GetMouseClickedEvent()
	{
		return m_mouseClickedEvent;
	}


}