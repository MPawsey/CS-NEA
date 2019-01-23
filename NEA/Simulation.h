#pragma once

#include <vector>
#include "Car.h"

namespace Evolution
{
	namespace Simulation
	{

		void Init();
		void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount);

		void SetSeedText(unsigned int seed);
		void SetIteration(unsigned int iteration);
	}
}