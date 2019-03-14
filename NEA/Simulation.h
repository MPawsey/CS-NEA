#pragma once

#include <vector>
#include "Car.h"
#include <sfml/Graphics.hpp>

namespace Evolution
{
	class Simulation
	{
	private:
		sf::View m_uiView;
		sf::View m_networkView;
		sf::View m_simulationView;
		sf::Text m_seedText;
		sf::Text m_iterationText;
		Machine::Car* m_prevBestCar;

		void OnWindowResized(sf::Vector2u size);

	public:
		static const unsigned int SIMULATION_FRAMERATE = 500u;

		void Init();
		void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw);

		void SetSeedText(unsigned int seed);
		void SetIteration(unsigned int iteration);
	};
}