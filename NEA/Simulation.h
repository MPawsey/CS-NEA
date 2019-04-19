#pragma once

#include <vector>
#include "Car.h"
#include <sfml/Graphics.hpp>
#include "CheckBox.h"

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

		UI::CheckBox m_showCheckpointsCB;
		sf::Text m_showCheckpointsText;

		void OnWindowResized(sf::Vector2u size);

	public:
		static const unsigned int SIMULATION_FRAMERATE = 500u;

		void Init();
		void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw);
		void Load();
		void Unload();

		void SetSeedText(unsigned int seed);
		void SetIteration(unsigned int iteration);
	};
}