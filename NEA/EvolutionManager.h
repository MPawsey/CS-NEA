#pragma once

#include <string>
#include <random>

namespace Evolution
{
	namespace EvolutionManager
	{

		void Init();
		void Update();


		void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow, double mutPC, double splicePC, unsigned int seed);
		void CreateGenerationFromFile(std::string filename);

		void ResetCars();
		
		void StartNextGeneration(int cycleCount, bool draw);

		std::mt19937& GetRandomEngine();

	}
}