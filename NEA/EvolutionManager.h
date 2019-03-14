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
		bool m_analysis = false, m_canMultiReproduce = true;
		unsigned int m_aliveSize, m_iteration, m_saveSize = 0, m_killSize = 22;
		sf::View m_evolutionView;
		RNG::Random m_randomEngine;
		std::string m_track;
		float m_carWidth, m_carHeight, m_carRaySize;
		std::vector<unsigned int> m_carSizes;

		Analysis m_analysisScreen;
		Simulation m_simulationScreen;

		int m_cycleCount = 1;
		bool m_display = true;

		std::vector<Machine::Car*> m_cars;

		void Reset();

		~EvolutionManager();
	public:

		static EvolutionManager& GetEvolutionManager();

		void Init();
		void Update();


		void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow, double mutPC, double splicePC, unsigned int seed);
		void CreateGenerationFromFile(std::string filename);

		void ResetCars();
		
		void StartNextGeneration(int cycleCount, bool draw);

		void SaveGeneration(std::string filename);

		std::mt19937& GetRandomEngine();

	};
}