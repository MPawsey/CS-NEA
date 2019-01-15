#pragma once

#include <string>

namespace EvolutionManager
{

	void Update();


	void CreateGenerationFromSettings(float width, float height, unsigned int rayCount, float raySize, unsigned int popSize, float enginePow, float rotPow);
	void CreateGenerationFromFile(std::string filename);


	const unsigned int GetSeed();

}