#include "UI.h"


namespace UI
{
	// Private
	sf::Font m_font;



	// Public
	void Init()
	{
		m_font.loadFromFile("arial.ttf");
	}


	const sf::Font& GetFont()
	{
		return m_font;
	}

}
