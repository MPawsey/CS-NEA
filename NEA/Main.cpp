#include <iostream>
#include <sfml/Graphics.hpp>







int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, "Title" };


	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		window.clear();



		window.display();
	}


	return 0;
}