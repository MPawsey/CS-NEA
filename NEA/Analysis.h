#pragma once

#include <fstream>
#include <vector>

namespace Evolution
{
	namespace Analysis
	{

		void Init();
		void Update();

		void Load();
		void Unload();

		void SetGraph(std::vector<float> positions);
		void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin);
		void SaveGraph(std::ofstream& file);

	}
}