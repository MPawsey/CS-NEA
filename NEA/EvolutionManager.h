#pragma once

#include <string>
#include <random>
#include "Simulation.h"
#include "RNG.h"
#include "Analysis.h"

namespace Evolution
{

	class EvolutionManager
	{
	private:
		// Other
		RNG::Random m_randomEngine;
		std::string m_trackName;

		// The cars (and other information)
		bool m_canMultiReproduce = true;
		unsigned int m_aliveSize = 25, m_saveSize = 0, m_killSize = 22, m_iteration;
		float m_carWidth, m_carHeight, m_carRaySize;
		std::vector<unsigned int> m_carSizes;
		std::vector<Machine::Car*> m_cars;
		int m_cycleCount = 1;

		// The screens
		Analysis m_analysisScreen;
		Simulation m_simulationScreen;
		bool m_analysis = false;
		bool m_display = true;

		
		// Resets the values in the evolution state to their initial values
		void Reset();

		// Creates the next generation of cars
		void CreateNextGeneration();

		~EvolutionManager();
	public:

		// Returns the EvolutionManager used throughout the program
		static EvolutionManager& GetEvolutionManager();

		// Initialises the values for the evolution manager
		void Init();

		// Updates the evolution, including the drawing to the window
		void Update();

		// Applies the advanced settings to the cars (done before CreateGenerationFromSettings)
		void SetAdvancedSettings(unsigned int popSize, unsigned int saveSize, unsigned int killSize, bool canMultiReproduce, std::vector<unsigned int> nnSizes);

		// Creates a generation of cars from a series of settings
		void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, double mutateSize, float enginePow,
			float rotPow, double mutPC, double splicePC, unsigned int seed);

		// Creates a generation of cars from a file
		void CreateGenerationFromFile(std::string filename);

		// Returns the population size
		unsigned int GetPopulationSize();

		// Resets the cars to the initial state for the race track
		void ResetCars();
		
		// Starts the next generation
		// Performs as many generations as cycleCount
		// Only draws to the screen if draw is true, remains on the analysis screen otherwise
		void StartNextGeneration(int cycleCount, bool draw);

		// Saves the current generation to a given file
		void SaveGeneration(std::string filename);

		// Gets the random engine for RNG
		std::mt19937& GetRandomEngine();

		// Initialises the simulation for this set of cars
		// Only to be used on gen 0
		void LoadInitialSimulation();
	};
}