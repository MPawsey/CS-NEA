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
	// Merge sort for a list of cars
	// Sorts such that the cars passed in becomes best to worst fitness
	void MergeSortCars(std::vector<Machine::Car*>& cars)
	{
		// Recursion check
		// Stops going deeper if the length of cars is 1
		if (cars.size() > 1)
		{
			// Finds the middle of the cars list
			size_t mid = cars.size() / 2;
			
			// Splits the cars list in half (left half including the middle)
			std::vector<Machine::Car*> leftHalf{ cars.begin(), cars.begin() + mid };
			std::vector<Machine::Car*> rightHalf{ cars.begin() + mid, cars.end() };

			// Calls merge sort on each half
			MergeSortCars(leftHalf);
			MergeSortCars(rightHalf);

			// Creates 3 temporary index variables setting all to 0
			// i indexes leftHalf
			// j indexes rightHalf
			// k indexes cars
			int i, j, k;
			i = j = k = 0;

			// Loops until the indexes for the halves exceeds the size of the vectors
			while (i < leftHalf.size() && j < rightHalf.size())
			{
				// Adds the car with the lower fitness
				if (leftHalf[i]->GetFitness() > rightHalf[j]->GetFitness())
					cars[k++] = leftHalf[i++];
				else
					cars[k++] = rightHalf[j++];
			}

			// Adds the leftover cars from left and right halves (if needed)
			while (i < leftHalf.size())
				cars[k++] = leftHalf[i++];

			while (j < rightHalf.size())
				cars[k++] = rightHalf[j++];
		}
	}

	// Returns a static instance of EvolutionManager to be used by the program
	EvolutionManager& EvolutionManager::GetEvolutionManager()
	{
		// Gets the static instance
		static EvolutionManager evolutionManager{};
		return evolutionManager;
	}
	
	// Resets the values of the evolution manager to their initial states
	void EvolutionManager::Reset()
	{
		m_cars.clear();
		m_analysisScreen.Reset();
		m_cycleCount = 1;
		m_display = true;
		trackName = "";
	}

	// Creates the next generation of cars to be simulated
	void EvolutionManager::CreateNextGeneration()
	{
		// Creates a vector for the new generation of cars
		std::vector<Machine::Car*> newCars;
		newCars.reserve(m_aliveSize);

		// Saves the necessary cars for the next generation
		for (unsigned int i = 0; i < m_saveSize; i++)
			newCars.push_back(new Machine::Car(*m_cars[i]));

		// Kills of the necessary cars
		for (unsigned int i = 0; i < m_killSize; i++)
		{
			// Deletes the car to free memory
			delete m_cars.back();
			m_cars.pop_back();
		}

		// Loops until the number of cars in the new generation is what it should be, or until there are no pairs left
		while (m_cars.size() > 1 && newCars.size() < m_aliveSize)
		{
			// Creates a vector of 2 cars sampled from the cars vector
			std::vector<Machine::Car*> c;
			std::sample(m_cars.begin(), m_cars.end(), std::back_inserter(c), 2, m_randomEngine);

			// Gets the cars from the sample and splices them
			Machine::Car* c1 = new Machine::Car(*c[0]);
			Machine::Car* c2 = new Machine::Car(*c[1]);
			Machine::Car::SpliceCars(*c1, *c2);

			// Adds the new car(s) to the new generation if needed
			newCars.push_back(c1);
			if (newCars.size() < m_aliveSize)
				newCars.push_back(c2);

			// Removes the cars from the old generation if they can only reproduce once
			if (!m_canMultiReproduce)
			{
				// Deletes the cars to free up memory
				delete c[0];
				delete c[1];
				m_cars.erase(std::find(m_cars.begin(), m_cars.end(), c[0]));
				m_cars.erase(std::find(m_cars.begin(), m_cars.end(), c[1]));
			}
		}

		// Adds the remaining car if required
		if (m_cars.size() == 1)
		{
			newCars.push_back(m_cars[0]);
		}

		// Mutates the new cars and creates their network diagrams
		// Resets all cars
		for (unsigned int i = 0; i < newCars.size(); i++)
		{
			if (i >= m_saveSize)
			{
				newCars[i]->Mutate();
				newCars[i]->GetNeuralNetwork().CreateNetworkDiagram();
			}
			newCars[i]->Reset();
		}

		// Sets the cars for the next generation
		m_cars = newCars;
	}

	// Destructor for the evolution manager
	EvolutionManager::~EvolutionManager()
	{
		// Deletes the cars in the cars vector
		for (Machine::Car* car : m_cars)
			delete car;
	}

	// Initialises the values for the evolution manager
	void EvolutionManager::Init()
	{
		// Initialises the screens for the evolution state
		m_simulationScreen.Init();
		m_analysisScreen.Init();

		// Initialises base values for the car network sizes
		m_carSizes = { 4, 3, 2 };
	}

	// Updates the evolution manager (includes drawing to the window)
	void EvolutionManager::Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Checks whether the program is simulating the cars
		if (!m_analysis)
		{
			// Updates the simulation
			m_simulationScreen.Update(m_cars, m_aliveSize, m_display);

			// If the generation is complete
			if (m_aliveSize == 0)
			{
				// Updates the iteration
				m_simulationScreen.SetIteration(++m_iteration);

				// Resets the number of cars alive
				m_aliveSize = m_cars.size();

				// Sorts the cars from best to worst
				MergeSortCars(m_cars);

				// Calculates the average fitness and updates the graph in the analysis screen
				float avgFitness = std::accumulate(m_cars.begin(), m_cars.end(), 0, [](float val, Machine::Car* car) { return val + car->GetFitness(); }) / m_aliveSize;
				m_analysisScreen.UpdateGraph(m_cars.front()->GetFitness(), avgFitness, m_cars.back()->GetFitness());

				// Checks whether the analysis phase should be skipped
				if (--m_cycleCount == 0)
				{
					// Unloads the simulation and loads the analysis screen
					m_simulationScreen.Unload();
					m_analysisScreen.Load();
					m_analysis = true;
				}
				else
				{
					// Creates the next generation of cars to continue simulating
					CreateNextGeneration();
				}
			}
			
			// Updates the analysis screen if the simulation is not being drawn
			if (!m_display)
			{
				m_analysisScreen.Update();
			}
		}
		else
		{
			// Updates the analysis screen
			m_analysisScreen.Update();
		}
	}

	// Sets the advanced car settings
	void EvolutionManager::SetAdvancedSettings(unsigned int popSize, unsigned int saveSize, unsigned int killSize, bool canMultiReproduce, std::vector<unsigned int> nnSizes)
	{
		m_aliveSize = popSize;
		m_carSizes = nnSizes;
		m_killSize = killSize;
		m_saveSize = saveSize;
		m_canMultiReproduce = canMultiReproduce;
	}

	// Creates a generation of cars from the given settings
	// This should be finalised after advanced settings has been set
	void EvolutionManager::CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, double mutateSize, float enginePow, float rotPow,
		double mutPC, double splicePC, unsigned int seed)
	{
		// Resets the values of the evolution manager
		Reset();
		m_analysis = false;

		// Sets the car values
		m_carWidth = width;
		m_carHeight = height;
		m_carRaySize = raySize;
		m_carSizes.insert(m_carSizes.begin(), rayCount);

		// Updates the iteration and seed
		m_iteration = 0;
		m_simulationScreen.SetIteration(m_iteration);
		m_simulationScreen.SetSeedText(seed);
		m_randomEngine.SetSeed(seed);

		// Sets the static values for the car and neurons
		Machine::Car::enginePower = enginePow;
		Machine::Car::rotationPower = rotPow;
		Machine::Neuron::mutateSize = mutateSize;
		Machine::Neuron::mutatePC = mutPC;
		Machine::Neuron::splicePC = splicePC;

		// Creates the rays for the cars
		Machine::Car::CreateRays(rayCount, raySize, width, height);

		// Creates a new randomised generation of cars
		for (unsigned int i = 0; i < m_aliveSize; i++)
		{
			m_cars.push_back(new Machine::Car{ width, height, m_carSizes });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
	}

	// Creates a generation of cars from a file
	void EvolutionManager::CreateGenerationFromFile(std::string filename)
	{
		// Resets the values of the evolution manager
		Reset();

		// Loads the analysis screen
		m_analysis = true;
		m_analysisScreen.Load();

		// Gets the file
		std::ifstream file{ filename };

		// Temporary buffer variables
		unsigned int seed;
		std::vector<std::vector<std::vector<std::vector<double>>>> networkWeights;
		std::vector<std::vector<std::vector<double>>> networkBiases;

		// Checks if the file opened
		if (file.is_open())
		{
			// Loops through the file until the end
			while (!file.eof())
			{
				// Gets the next line in the file
				std::string s;
				std::getline(file, s);
				std::stringstream ss;
				ss << s;

				// Variable to store the first character of each line
				char junk;

				// Determine what the line is storing and process it
				if (s[0] == 't') // Track & Seed & iteration
				{
					unsigned long long count;
					ss >> junk >> trackName >> seed >> count >> m_iteration;
					m_randomEngine.SetSeed(seed);
					m_randomEngine.discard(count);
				}
				else if (s[0] == 'd') // Dimensions
				{
					ss >> junk >> m_carWidth >> m_carHeight;
				}
				else if (s[0] == 's') // Sizes
				{
					// Clears the network sizes for the cars
					m_carSizes.clear();
					
					// Creates a vector to store each space seperated element in the file
					// Starts the vector with a blank string
					std::vector<std::string> parts;
					parts.push_back("");

					// Continually reads the line until its end
					for (char c : s)
					{
						// Checks whether the current character is a space
						if (c == ' ')
						{
							// Pushes a new blank string to the back of the parts list
							parts.push_back("");
						}
						else
						{
							// Adds the character to the back of the last element of the parts list
							parts.back().push_back(c);
						}
					}

					// Converts the part to an integer and add it to the network size list
					// Starts at 1 as first part will be the junk char
					for (unsigned int i = 1; i < parts.size(); i++)
					{
						// Skips if the part is a blank string
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
				else if (s[0] == 'g') // Graph
				{
					// Creates a list of positions and a temporary val string to store the current element of the string
					std::vector<float> positions;
					std::string val;
					
					// Go through each character in the line, excluding the first two (should be "g " which can be ignored)
					for (auto i = s.begin() + 2; i < s.end(); i++)
					{
						// Gets the current character
						std::string c{ *i };

						// Checks if the current character is a space
						if (c == " ")
						{
							// Adds the new position to the positions vector (if valid)
							if (val.size() > 0)
							{
								positions.push_back(std::stod(val));

								// Clears val for the next value in the list
								val.clear();
							}
						}
						else
						{
							// Appends the character to the val
							val.append(c);
						}
					}

					// Checks whether val still stores a value and add it to
					// the positions list if it does
					if (val != "" && val != " ")
					{
						positions.push_back(std::stod(val));
					}

					// Sets the graph for the analysis screen
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
					// Temporary variables for the weights and bias and current value
					std::vector<double> weights;
					double bias;
					std::string val;

					// Go through each character in the line, excluding the first two (should be "n " which can be ignored)
					for (auto i = s.begin() + 2; i < s.end(); i++)
					{
						// Gets the current character
						std::string c{ *i };

						// Checks if the current character is a space
						if (c == " ")
						{
							// Adds the new weight to the weights vector (if valid)
							if (val.size() > 0)
							{
								weights.push_back(std::stod(val));

								// Clears val for the next value in the list
								val.clear();
							}
						}
						else
						{
							// Appends the character to the val
							val.append(c);
						}
					}

					// Checks whether val still contains a value
					if (val != "" && val != " ")
					{
						// Sets the bais to value
						bias = std::stod(val);
					}
					else
					{
						// Sets the bias to the final value in the weights list, and remove the final value from said list
						bias = weights.back();
						weights.pop_back();
					}

					// Add the weights and bias to the network
					networkWeights.back().back().push_back(weights);
					networkBiases.back().back().push_back(bias);
				}
			}
		}

		// Sets the UI for the iteration and seed
		m_simulationScreen.SetIteration(m_iteration);
		m_simulationScreen.SetSeedText(seed);

		// Creates the rays for the cars
		Machine::Car::CreateRays(m_carSizes[0], m_carRaySize, m_carWidth, m_carHeight);

		// Creates the cars from the settings
		for (unsigned int i = 0; i < m_aliveSize; i++)
		{
			m_cars.push_back(new Machine::Car{ m_carWidth, m_carHeight, m_carSizes, networkWeights[i], networkBiases[i] });
			m_cars.back()->GetNeuralNetwork().CreateNetworkDiagram();
		}
	}

	// Returns the number of cars in the population
	unsigned int EvolutionManager::GetPopulationSize()
	{
		return m_cars.size();
	}

	// Resets all of the cars in the generation and update the iteration text
	void EvolutionManager::ResetCars()
	{
		m_simulationScreen.SetIteration(m_iteration);
		for (auto* car : m_cars)
			car->Reset();
	}

	// Starts the next generation
	// Simulates as many generations as equal to cycleCount
	// Only draws the simulation if draw is true
	void EvolutionManager::StartNextGeneration(int cycleCount, bool draw)
	{
		// Sets the cycle count
		m_cycleCount = cycleCount;

		// Creates the next generation of cars to be simulated
		CreateNextGeneration();
		
		// Sets whether the simulation should be displayed, and acts accordingly
		if (m_display = draw)
		{
			// Loads the simulation screen
			Window::GetWindow().setFramerateLimit(m_simulationScreen.SIMULATION_FRAMERATE);
			m_simulationScreen.Load();
		}
		else
		{
			// Sets the framerate of the window to unlimited
			Window::GetWindow().setFramerateLimit(0);
		}

		// Unloads the analysis screen
		m_analysis = false;
		m_analysisScreen.Unload();
	}
	
	// Saves the current generation of cars to a given file
	void EvolutionManager::SaveGeneration(std::string filename)
	{
		// Gets the filepath and file to save the cars
		std::string filepath = "Cars/" + filename + ".cars";
		std::ofstream file = std::ofstream{ filepath };

		// Saving the settings of the cars
		Functions::WriteLineToFile(file, 't', RaceTrack::GetTrackName(), m_randomEngine.GetSeed(), m_randomEngine.GetCalls(), m_iteration);
		Functions::WriteLineToFile(file, 'd', m_carWidth, m_carHeight);
		// Converts the network sizes to a space seperated list in string format
		std::string sizeStr = std::accumulate(m_carSizes.begin(), m_carSizes.end(), std::string{ "" }, [](std::string val, unsigned int cur) { return val.append(std::to_string(cur) + " "); });
		Functions::WriteLineToFile(file, 's', (sizeStr.erase(sizeStr.end() - 1), sizeStr));
		Functions::WriteLineToFile(file, 'r', m_carRaySize);
		Functions::WriteLineToFile(file, 'p', m_cars.size(), m_saveSize, m_killSize, m_canMultiReproduce);
		Functions::WriteLineToFile(file, 'e', Machine::Car::enginePower, Machine::Car::rotationPower);
		Functions::WriteLineToFile(file, 'o', Machine::Neuron::mutatePC, Machine::Neuron::splicePC);
		// Saves the graph
		m_analysisScreen.SaveGraph(file);

		// Iterates through the cars and saves them
		for (Machine::Car* car : m_cars)
		{
			Functions::WriteLineToFile(file, *car);
		}

	}

	// Returns the random number engine
	std::mt19937& EvolutionManager::GetRandomEngine()
	{
		return m_randomEngine;
	}

	// Loads the initial simulation (for gen 0)
	void EvolutionManager::LoadInitialSimulation()
	{
		// Loads the simulation screen
		m_simulationScreen.Load();

		// Sets the finish line of the track for the graph
		m_analysisScreen.SetFinishLine(RaceTrack::GetTrackDistance());

		// Checks whether the graph should be carried through from a file
		if (RaceTrack::GetTrackName() != trackName)
		{
			m_analysisScreen.SetGraph({});
			m_simulationScreen.SetIteration(m_iteration = 0);
		}
	}
}