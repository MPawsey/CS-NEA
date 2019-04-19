#include "EvolutionManager.h"
#include <vector>
#include "Car.h"
#include <numeric>
#include "Window.h"
#include "RaceTrack.h"
#include "Analysis.h"
#include <sstream>
#include "Functions.h"

namespace Evolution
{
	

	EvolutionManager& EvolutionManager::GetEvolutionManager()
	{
		static EvolutionManager evolutionManager{};
		return evolutionManager;
	}
	

	void EvolutionManager::Reset()
	{
		m_cars.clear();
		m_analysisScreen.Reset();
		m_cycleCount = 1;
		m_display = true;
	}

	EvolutionManager::~EvolutionManager()
	{
		for (Machine::Car* car : m_cars)
			delete car;
	}

	void EvolutionManager::Init()
	{
		m_simulationScreen.Init();

		m_evolutionView = Window::GetWindow().getDefaultView();
		m_evolutionView.setSize((sf::Vector2f)Window::GetWindowSize());
		m_evolutionView.setCenter(0.f, 0.f);

		m_analysisScreen.Init();

		m_carSizes = { 4, 3, 2 };
	}

	void EvolutionManager::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		if (!m_analysis)
		{
			m_simulationScreen.Update(m_cars, m_aliveSize, m_display);

			// If the generation is complete
			if (m_aliveSize == 0)
			{
				m_simulationScreen.SetIteration(++m_iteration);

				m_aliveSize = m_cars.size();

				std::sort(m_cars.begin(), m_cars.end(), [](Machine::Car* lhs, Machine::Car* rhs)
				{
					return lhs->GetFitness() > rhs->GetFitness();
				});

				float avgFitness = std::accumulate(m_cars.begin(), m_cars.end(), 0, [](float val, Machine::Car* car) { return val + car->GetFitness(); }) / m_aliveSize;

				m_analysisScreen.UpdateGraph(m_cars.front()->GetFitness(), avgFitness, m_cars.back()->GetFitness());

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
					m_analysisScreen.Load();
					m_analysis = true;
				}
			}
			
			if (!m_display)
			{
				m_analysisScreen.Update();
			}
		}
		else
		{
			m_analysisScreen.Update();
		}
	}

	void EvolutionManager::SetAdvancedSettings(unsigned int popSize, unsigned int saveSize, unsigned int killSize, bool canMultiReproduce, std::vector<unsigned int> nnSizes)
	{
		m_aliveSize = popSize;
		m_carSizes = nnSizes;
		m_killSize = killSize;
		m_saveSize = saveSize;
		m_canMultiReproduce = canMultiReproduce;
	}

	void EvolutionManager::CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, double mutateSize, float enginePow, float rotPow,
		double mutPC, double splicePC, unsigned int seed)
	{
		Reset();
		m_analysis = false;

		m_carWidth = width;
		m_carHeight = height;
		m_carRaySize = raySize;
		m_carSizes.insert(m_carSizes.begin(), rayCount);

		m_iteration = 0;
		m_simulationScreen.SetIteration(m_iteration);
		m_simulationScreen.SetSeedText(seed);


		Machine::Car::enginePower = enginePow;
		Machine::Car::rotationPower = rotPow;
		Machine::Neuron::mutateSize = mutateSize;
		Machine::Neuron::mutatePC = mutPC;
		Machine::Neuron::splicePC = splicePC;

		m_randomEngine.SetSeed(seed);

		Machine::Car::CreateRays(rayCount, raySize, width, height);

		for (unsigned int i = 0; i < m_aliveSize; i++)
		{
			m_cars.push_back(new Machine::Car{ width, height, m_carSizes });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
	}

	void EvolutionManager::CreateGenerationFromFile(std::string filename)
	{
		Reset();
		m_analysis = true;
		m_analysisScreen.Load();

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
				else if (s[0] == 'p') // Population Size, save, kill and canMultiReprod
				{
					ss >> junk >> m_aliveSize >> m_saveSize >> m_killSize >> m_canMultiReproduce;
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

					m_analysisScreen.SetGraph(positions);
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

		m_simulationScreen.SetIteration(m_iteration);
		m_simulationScreen.SetSeedText(seed);

		Machine::Car::CreateRays(m_carSizes[0], m_carRaySize, m_carWidth, m_carHeight);

		for (unsigned int i = 0; i < m_aliveSize; i++)
		{
			m_cars.push_back(new Machine::Car{ m_carWidth, m_carHeight, m_carSizes, networkWeights[i], networkBiases[i] });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
	}

	unsigned int EvolutionManager::GetPopulationSize()
	{
		return m_cars.size();
	}

	void EvolutionManager::ResetCars()
	{
		m_simulationScreen.SetIteration(m_iteration);
		for (auto* car : m_cars)
			car->Reset();
	}

	void EvolutionManager::StartNextGeneration(int cycleCount, bool draw)
	{
		m_cycleCount = cycleCount;
		
		if (m_display = draw)
		{
			Window::GetWindow().setFramerateLimit(m_simulationScreen.SIMULATION_FRAMERATE);
		}
		else
		{
			Window::GetWindow().setFramerateLimit(0);
		}

		m_analysis = false;
		m_analysisScreen.Unload();
	}
	
	void EvolutionManager::SaveGeneration(std::string filename)
	{
		std::string filepath = "Cars/" + filename + ".cars";


		std::ofstream file = std::ofstream{ filepath };

		// Saving the settings of the cars
		Functions::WriteLineToFile(file, 't', RaceTrack::GetTrackName(), m_randomEngine.GetSeed(), m_randomEngine.GetCalls(), m_iteration);
		Functions::WriteLineToFile(file, 'd', m_carWidth, m_carHeight);
		std::string sizeStr = std::accumulate(m_carSizes.begin(), m_carSizes.end(), std::string{ "" }, [](std::string val, unsigned int cur) { return val.append(std::to_string(cur) + " "); });
		Functions::WriteLineToFile(file, 's', (sizeStr.erase(sizeStr.end() - 1), sizeStr));
		Functions::WriteLineToFile(file, 'r', m_carRaySize);
		Functions::WriteLineToFile(file, 'p', m_cars.size(), m_saveSize, m_killSize, m_canMultiReproduce);
		Functions::WriteLineToFile(file, 'e', Machine::Car::enginePower, Machine::Car::rotationPower);
		Functions::WriteLineToFile(file, 'o', Machine::Neuron::mutatePC, Machine::Neuron::splicePC);
		m_analysisScreen.SaveGraph(file);

		for (Machine::Car* car : m_cars)
		{
			Functions::WriteLineToFile(file, *car);
		}

	}

	std::mt19937& EvolutionManager::GetRandomEngine()
	{
		return m_randomEngine;
	}
}