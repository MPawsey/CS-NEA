#pragma once

#include <string>
#include <random>

namespace EvolutionManager
{

	void Init();
	void Update();


	void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow);
	void CreateGenerationFromFile(std::string filename);


	const std::default_random_engine& GetRandomEngine();

}