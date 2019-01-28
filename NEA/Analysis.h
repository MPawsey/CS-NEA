#pragma once

namespace Evolution
{
	namespace Analysis
	{

		void Init();
		void Update();

		void Load();
		void Unload();

		void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin);


	}
}