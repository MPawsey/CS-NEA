#pragma once

#include <vector>
#include "Car.h"

namespace Evolution
{
	namespace Simulation
	{
		constexpr unsigned int SIMULATION_FRAMERATE = 500u;

		void Init();
		void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw);

		void SetSeedText(unsigned int seed);
		void SetIteration(unsigned int iteration);
	}
}