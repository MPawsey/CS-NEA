#include "EvolutionManager.h"
#include <vector>
#include "Car.h"
#include <numeric>
#include "Window.h"
#include "RaceTrack.h"
#include "Simulation.h"
#include "Analysis.h"
#include <sstream>
#include <fstream>
#include "RNG.h"

namespace Evolution::EvolutionManager
{
	bool m_analysis = false, m_canMultiReproduce = true;
	unsigned int m_aliveSize, m_iteration, m_saveSize = 0, m_killSize = 22;
	sf::View m_evolutionView;
	RNG::Random m_randomEngine;
	std::string m_track;
	float m_carWidth, m_carHeight, m_carRaySize;
	std::vector<unsigned int> m_carSizes;
	//std::mt19937 m_randomEngine;

	int m_cycleCount = 1;
	bool m_display = true;

	std::vector<Machine::Car*> m_cars;

	void Reset()
	{
		m_cars.clear();
		Analysis::Reset();
		m_cycleCount = 1;
		m_display = true;
	}

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
		Reset();
		m_analysis = false;

		m_carWidth = width;
		m_carHeight = height;
		m_carRaySize = raySize;
		m_carSizes = { rayCount, 4, 3, 2};

		m_iteration = 0;
		Simulation::SetIteration(m_iteration);
		Simulation::SetSeedText(seed);

		m_aliveSize = popSize;

		Machine::Car::enginePower = enginePow;
		Machine::Car::rotationPower = rotPow;
		Machine::Neuron::mutatePC = mutPC;
		Machine::Neuron::splicePC = splicePC;

		m_randomEngine.SetSeed(seed);

		Machine::Car::CreateRays(rayCount, raySize, width, height);

		for (unsigned int i = 0; i < popSize; i++)
		{
			m_cars.push_back(new Machine::Car{ width, height, m_carSizes });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
	}

	void CreateGenerationFromFile(std::string filename)
	{
		Reset();
		m_analysis = true;
		Analysis::Load();

		std::ifstream file{ filename };

		std::string track;
		unsigned int seed;
		std::vector<std::vector<std::vector<std::vector<double>>>> networkWeights;
		std::vector<std::vector<std::vector<double>>> networkBiases;

		if (file.is_open())
		{
			// Parse file
			while (!file.eof())
			{
				std::string s;

				std::getline(file, s);
				std::stringstream ss;
				ss << s;

				char junk;


				if (s[0] == 't') // Track & Seed & iteration
				{
					unsigned int count;
					ss >> junk >> track >> seed >> count >> m_iteration;
					m_randomEngine.SetSeed(seed);
					m_randomEngine.discard(count);
				}
				else if (s[0] == 'd') // Dimensions
				{
					ss >> junk >> m_carWidth >> m_carHeight;
				}
				else if (s[0] == 's') // Sizes
				{
					m_carSizes.clear();
					std::vector<std::string> parts;
					parts.push_back("");
					for (char c : s)
					{
						if (c == ' ')
						{
							parts.push_back("");
						}
						else
						{
							parts.back().push_back(c);
						}
					}
					for (unsigned int i = 1; i < parts.size(); i++)
					{
						if (parts[i] == "")
							continue;
						m_carSizes.push_back(std::stoi(parts[i]));
					}
				}
				else if (s[0] == 'r') // Ray Size
				{
					ss >> junk >> m_carRaySize;
				}
				else if (s[0] == 'p') // Population Size
				{
					ss >> junk >> m_aliveSize;
				}
				else if (s[0] == 'e') // Engine & Rotation Power
				{
					ss >> junk >> Machine::Car::enginePower >> Machine::Car::rotationPower;
				}
				else if (s[0] == 'o') // Offspring settings
				{
					ss >> junk >> Machine::Neuron::mutatePC >> Machine::Neuron::splicePC;
				}
				else if (s[0] == 'g')
				{
					std::vector<float> positions;

					std::string val;
					for (auto i = s.begin() + 2; i < s.end(); i++)
					{
						std::string c{ *i };
						if (c == " ")
						{
							if (val.size() > 0)
							{
								positions.push_back(std::stod(val));
								val.clear();
							}
						}
						else
						{
							val.append(c);
						}
					}

					if (val != "" && val != " ")
					{
						positions.push_back(std::stod(val));
					}

					Analysis::SetGraph(positions);
				}
				else if (s[0] == 'c') // Sets new values for layers and neurons to new car
				{
					networkWeights.push_back({});
					networkBiases.push_back({});
				}
				else if (s[0] == 'l') // Sets new values for neurons to new layer
				{
					networkWeights.back().push_back({});
					networkBiases.back().push_back({});
				}
				else if (s[0] == 'n') // Sets values for a single neuron index equal inline with number created between this one and the last 'l'
				{
					std::vector<double> weights;
					double bias;

					std::string val;
					for (auto i = s.begin() + 2; i < s.end(); i++)
					{
						std::string c{ *i };
						if (c == " ")
						{
							if (val.size() > 0)
							{
								weights.push_back(std::stod(val));
								val.clear();
							}
						}
						else
						{
							val.append(c);
						}
					}
					if (val != "" && val != " ")
					{
						bias = std::stod(val);
					}
					else
					{
						bias = weights.back();
						weights.pop_back();
					}

					networkWeights.back().back().push_back(weights);
					networkBiases.back().back().push_back(bias);
				}
			}
		}

		Simulation::SetIteration(m_iteration);
		Simulation::SetSeedText(seed);

		Machine::Car::CreateRays(m_carSizes[0], m_carRaySize, m_carWidth, m_carHeight);

		for (unsigned int i = 0; i < m_aliveSize; i++)
		{
			m_cars.push_back(new Machine::Car{ m_carWidth, m_carHeight, m_carSizes, networkWeights[i], networkBiases[i] });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
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

	// Fold expression to write a line to a file
	template <typename... Args>
	void WriteLineToFile(std::ofstream& file, Args&&... args)
	{
		// Folds over the first comma seperated expression resulting in a trailing space (should not matter)
		((file << args << " "), ...) << std::endl;
	}
	
	void SaveGeneration(std::string filename)
	{
		std::string filepath = "Cars/" + filename + ".cars";


		std::ofstream file = std::ofstream{ filepath };

		// Saving the settings of the cars
		WriteLineToFile(file, 't', RaceTrack::GetTrackName(), m_randomEngine.GetSeed(), m_randomEngine.GetCalls(), m_iteration);
		WriteLineToFile(file, 'd', m_carWidth, m_carHeight);
		std::string sizeStr = std::accumulate(m_carSizes.begin(), m_carSizes.end(), std::string{ "" }, [](std::string val, unsigned int cur) { return val.append(std::to_string(cur) + " "); });
		WriteLineToFile(file, 's', (sizeStr.erase(sizeStr.end() - 1), sizeStr));
		WriteLineToFile(file, 'r', m_carRaySize);
		WriteLineToFile(file, 'p', m_cars.size());
		WriteLineToFile(file, 'e', Machine::Car::enginePower, Machine::Car::rotationPower);
		WriteLineToFile(file, 'o', Machine::Neuron::mutatePC, Machine::Neuron::splicePC);
		Analysis::SaveGraph(file);

		for (Machine::Car* car : m_cars)
		{
			WriteLineToFile(file, *car);
		}

	}

	std::mt19937& GetRandomEngine()
	{
		return m_randomEngine;
	}
}