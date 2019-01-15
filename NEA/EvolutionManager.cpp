#include "EvolutionManager.h"
#include <vector>
#include "Car.h"

#include "Logger.h"
#include "Window.h"

namespace EvolutionManager
{
	Logger logger{ "EvolutionManager" };

	unsigned int m_aliveSize;
	unsigned int m_seed;

	std::vector<Machine::Car> m_cars;


	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		if (m_aliveSize > 0)
		{
			for (Machine::Car& car : m_cars)
			{
				if (car.IsAlive() && car.Update())
					m_aliveSize--;
				window.draw(car);
			}
		}
		else
		{
			// DO STUFF
		}
	}


	void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow)
	{
		m_aliveSize = popSize;

		Machine::Car::enginePower = enginePow;
		Machine::Car::rotationPower = rotPow;
		Machine::Car::CreateRays(rayCount, raySize, width, height);

		for (unsigned int i = 0; i < popSize; i++)
		{
			m_cars.emplace_back(width, height, rayCount);
		}
	}

	void CreateGenerationFromFile(std::string filename)
	{

	}

	const unsigned int GetSeed()
	{
		return 0;
	}
}