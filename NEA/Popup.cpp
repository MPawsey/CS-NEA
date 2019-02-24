#include "Popup.h"

namespace UI
{

	Popup::Popup()
	{
		m_background.setFillColor(sf::Color{ 40, 46, 71, 255 });
	}

	Popup::Popup(sf::View& view)
		: UIElement{ view }
	{
		m_background.setFillColor(sf::Color{ 40, 46, 71, 255 });
	}

	Popup::Popup(sf::View* view)
		: UIElement{ view }
	{
		m_background.setFillColor(sf::Color{ 40, 46, 71, 255 });
	}

	Popup::~Popup()
	{
		for (UIElement* element : m_elementList)
		{
			delete element;
		}
	}

	Popup& Popup::operator=(const Popup& popup)
	{
		m_containerView = popup.m_containerView;
		m_elementList = popup.m_elementList;
		m_background = popup.m_background;
		return *this;
	}

	void Popup::AddElement(UIElement* element)
	{
		m_elementList.push_back(element);
	}

	void Popup::RemoveElement(UIElement* element)
	{
		if (auto e = std::find(m_elementList.begin(), m_elementList.end(), element); e != m_elementList.end())
		{
			m_elementList.erase(e);
		}
	}

	void Popup::SetViewportTransform(sf::FloatRect rect)
	{
		m_view.setViewport(rect);
	}
	
	void Popup::SetViewSize(sf::Vector2f size)
	{
		m_background.setSize(size);
		m_view.setSize(size);
		m_view.setCenter(size / 2.f);
	}

	void Popup::SetActive(bool isActive)
	{
		if (m_isVisible = isActive)
		{
			for (UIElement* element : m_elementList)
			{
				element->SetActive(true);
			}
		}
		else
		{
			for (UIElement* element : m_elementList)
			{
				element->SetActive(false);
			}
		}
	}

	sf::View& Popup::GetView()
	{
		return m_view;
	}

	void Popup::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_isVisible)
		{
			const sf::View& tmpView = target.getView();
			target.setView(m_view);

			target.draw(m_background, states);
			for (UIElement* element : m_elementList)
			{
				target.draw(*element, states);
			}

			target.setView(tmpView);
		}
	}
}