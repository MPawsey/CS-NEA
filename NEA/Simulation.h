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
		// Views
		sf::View m_uiView, m_networkView, m_simulationView;

		// UI
		sf::Text m_seedText, m_iterationText, m_showCheckpointsText;
		UI::UICheckBox m_showCheckpointsCB;

		// Other
		Machine::Car* m_prevBestCar;

		// Events
		void OnWindowResized(sf::Vector2u size);

	public:
		// The max framerate for when displaying the simulation
		static const unsigned int SIMULATION_FRAMERATE = 500u;

		// Initialises all variables
		void Init();

		// Updates the simulation
		void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw);

		// Loads the simulation
		void Load();

		// Unloads the simulation
		void Unload();

		// Sets the seed being displayed during the simulation phase
		void SetSeedText(unsigned int seed);
		
		// Sets the iteration being displayed during the simulation phase
		void SetIteration(unsigned int iteration);
	};
}