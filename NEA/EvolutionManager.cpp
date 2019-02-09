#include "EvolutionManager.h"
#include <vector>
#include "Car.h"
#include <numeric>
#include "Logger.h"
#include "Window.h"
#include "RaceTrack.h"
#include "Simulation.h"
#include "Analysis.h"

namespace Evolution::EvolutionManager
{
	Logger logger{ "EvolutionManager" };

	bool m_analysis = false, m_canMultiReproduce = true;
	unsigned int m_aliveSize, m_iteration, m_saveSize = 0, m_killSize = 22;
	sf::View m_evolutionView;
	std::mt19937 m_randomEngine;

	int m_cycleCount = 1;
	bool m_display = true;

	std::vector<Machine::Car*> m_cars;

	void OnWindowClosed()
	{
		for (Machine::Car* car : m_cars)
			delete car;
	}

	void Init()
	{
		Simulation::Init();

		m_evolutionView = Window::GetWindow().getDefaultView();
		m_evolutionView.setSize((sf::Vector2f)Window::GetWindowSize());
		m_evolutionView.setCenter(0.f, 0.f);

		Window::GetWindowClosedEvent().AddCallback(OnWindowClosed);

		Analysis::Init();
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		if (!m_analysis)
		{
			Simulation::Update(m_cars, m_aliveSize, m_display);

			// If the generation is complete
			if (m_aliveSize == 0)
			{
				Simulation::SetIteration(++m_iteration);

				m_aliveSize = m_cars.size();

				std::sort(m_cars.begin(), m_cars.end(), [](Machine::Car* lhs, Machine::Car* rhs)
				{
					return lhs->GetFitness() > rhs->GetFitness();
				});

				float avgFitness = std::accumulate(m_cars.begin(), m_cars.end(), 0, [](float val, Machine::Car* car) { return val + car->GetFitness(); }) / m_aliveSize;

				Analysis::UpdateGraph(m_cars.front()->GetFitness(), avgFitness, m_cars.back()->GetFitness());

				std::vector<Machine::Car*> newCars;
				newCars.reserve(m_aliveSize);

				for (unsigned int i = 0; i < m_saveSize; i++)
					newCars.push_back(new Machine::Car(*m_cars[i]));

				for (unsigned int i = 0; i < m_killSize; i++)
				{
					delete m_cars.back();
					m_cars.pop_back();
				}

				while (m_cars.size() > 1 && newCars.size() < m_aliveSize)
				{
					std::vector<Machine::Car*> c;
					std::sample(m_cars.begin(), m_cars.end(), std::back_inserter(c), 2, m_randomEngine);

					Machine::Car* c1 = new Machine::Car(*c[0]);
					Machine::Car* c2 = new Machine::Car(*c[1]);
					Machine::Car::SpliceCars(*c1, *c2);

					newCars.push_back(c1);
					if (newCars.size() < m_aliveSize)
						newCars.push_back(c2);

					if (!m_canMultiReproduce)
					{
						delete c[0];
						delete c[1];
						m_cars.erase(std::find(m_cars.begin(), m_cars.end(), c[0]));
						m_cars.erase(std::find(m_cars.begin(), m_cars.end(), c[1]));
					}
				}

				if (m_cars.size() == 1)
				{
					newCars.push_back(m_cars[0]);
				}

				for (unsigned int i = 0; i < newCars.size(); i++)
				{
					if (i >= m_saveSize)
					{
						newCars[i]->Mutate();
						newCars[i]->GetNeuralNetwork().CreateNetworkDiagram();
					}
					newCars[i]->Reset();
				}

				m_cars = newCars;

				if (--m_cycleCount == 0)
				{
					Analysis::Load();
					m_analysis = true;
				}
			}
			
			if (!m_display)
			{
				Analysis::Update();
			}
		}
		else
		{
			Analysis::Update();
		}
	}


	void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow, double mutPC, double splicePC, unsigned int seed)
	{
		m_iteration = 0;
		Simulation::SetIteration(m_iteration);
		Simulation::SetSeedText(seed);

		m_aliveSize = popSize;

		Machine::Car::enginePower = enginePow;
		Machine::Car::rotationPower = rotPow;
		Machine::Neuron::mutatePC = mutPC;
		Machine::Neuron::splicePC = splicePC;

		m_randomEngine.seed(seed);

		Machine::Car::CreateRays(rayCount, raySize, width, height);

		for (unsigned int i = 0; i < popSize; i++)
		{
			m_cars.push_back(new Machine::Car{ width, height, { rayCount, 4, 3, 2 } });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}

		Window::GetWindow().setFramerateLimit(Simulation::SIMULATION_FRAMERATE);
	}

	void CreateGenerationFromFile(std::string filename)
	{

	}

	void ResetCars()
	{
		Simulation::SetIteration(m_iteration);
		for (auto* car : m_cars)
			car->Reset();
	}

	void StartNextGeneration(int cycleCount, bool draw)
	{
		m_cycleCount = cycleCount;
		
		if (m_display = draw)
		{
			Window::GetWindow().setFramerateLimit(Simulation::SIMULATION_FRAMERATE);
		}
		else
		{
			Window::GetWindow().setFramerateLimit(0);
		}

		m_analysis = false;
		Analysis::Unload();
	}

	std::mt19937& GetRandomEngine()
	{
		return m_randomEngine;
	}
}