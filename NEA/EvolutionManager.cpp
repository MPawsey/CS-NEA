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
	std::default_random_engine m_randomEngine;
	sf::View m_evolutionView;

	std::vector<Machine::Car> m_cars;

	void Init()
	{
		m_evolutionView = Window::GetWindow().getDefaultView();

		m_randomEngine = std::default_random_engine{ std::random_device()() };

	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		if (m_aliveSize > 0)
		{

			Machine::Car* bestCar = &m_cars[0];
			for (Machine::Car& car : m_cars)
			{
				if (car.IsAlive() && car.Update())
					m_aliveSize--;

				if (car.CalcFitness() > bestCar->GetFitness())
					bestCar = &car;

				window.draw(car);
			}

			m_evolutionView.setCenter(bestCar->GetPos());
			window.setView(m_evolutionView);
		}
		else
		{
			m_aliveSize = m_cars.size();

			std::sort(m_cars.begin(), m_cars.end(), [](Machine::Car& lhs, Machine::Car& rhs)
			{
				return lhs.GetFitness() > rhs.GetFitness();
			});

			std::vector<Machine::Car> newCars;
			newCars.reserve(m_aliveSize);

			for (unsigned int i = 0; i < m_aliveSize / 10; i++)
			{
				newCars.push_back(Machine::Car(m_cars[i]));
				m_cars.pop_back();
			}

			while (m_cars.size() > 1)
			{
				std::vector<Machine::Car> c;
				std::sample(m_cars.begin(), m_cars.end(), std::back_inserter(c), 2, std::mt19937(std::random_device{}()));
				Machine::Car c1 = Machine::Car(c[0]);
				Machine::Car c2 = Machine::Car(c[1]);
				newCars.push_back(c1);
				newCars.push_back(c2);
				m_cars.erase(std::find(cars.begin(), cars.end(), c[0]));
				m_cars.erase(std::find(cars.begin(), cars.end(), c[1]));
			}

			if (cars.size() == 1)
			{
				newCars.push_back(cars[0]);
			}

			for (unsigned int i = 0; i < newCars.size(); i++)
			{
				if (i >= PopulationSize / 10)
					newCars[i]->Mutate();
				newCars[i]->Reset();
			}

			cars = newCars;
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

	const std::default_random_engine& GetRandomEngine()
	{
		return m_randomEngine;
	}
}