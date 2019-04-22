#include "Simulation.h"
#include "Window.h"
#include "UI.h"
#include "RaceTrack.h"

namespace Evolution
{
	// Event for when the window is resized
	void Simulation::OnWindowResized(sf::Vector2u size)
	{
		// Corrects the size of the simulation
		m_simulationView.setSize((sf::Vector2f)size);

		// Updates the view for the UI
		m_uiView.setSize((sf::Vector2f)size);
		m_uiView.setCenter((sf::Vector2f)size / 2.f);
	}

	// Initialises the simulation screen
	void Simulation::Init()
	{
		// Gets the font for the UI
		const sf::Font& font = UI::GetFont();

		// Initialises the UI view
		m_uiView = m_simulationView = Window::GetWindow().getDefaultView();

		// Initialises the network view
		m_networkView.setSize(1.f, 1.f);
		m_networkView.setCenter(0.5f, 0.5f);
		m_networkView.setViewport(sf::FloatRect{ 0.7f, 0.f, 0.3f, 0.3f });

		// Initialises the seed text
		m_seedText.setFillColor(sf::Color::White);
		m_seedText.setOutlineColor(sf::Color::Black);
		m_seedText.setOutlineThickness(2.f);
		m_seedText.setFont(font);
		m_seedText.setPosition(10.f, 10.f);

		// Initialises the iteration text
		m_iterationText.setFillColor(sf::Color::White);
		m_iterationText.setOutlineColor(sf::Color::Black);
		m_iterationText.setOutlineThickness(2.f);
		m_iterationText.setFont(font);
		m_iterationText.setPosition(10.f, 10.f + font.getLineSpacing(32));

		// Initialises the show checkpoints text
		m_showCheckpointsText = sf::Text{ "    Show\nCheckpoints:", font };
		m_showCheckpointsText.setFillColor(sf::Color::White);
		m_showCheckpointsText.setOutlineColor(sf::Color::Black);
		m_showCheckpointsText.setOutlineThickness(2.f);
		m_showCheckpointsText.setPosition(10.f, 10.f + (font.getLineSpacing(32) * 3));

		// Initialises the show checkpoints checkbox
		m_showCheckpointsCB = UI::UICheckBox{ m_uiView };
		m_showCheckpointsCB.SetChecked(true);
		m_showCheckpointsCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { RaceTrack::SetCheckpointsVisible(checked); });
		m_showCheckpointsCB.setPosition(m_showCheckpointsText.getGlobalBounds().left + (m_showCheckpointsText.getGlobalBounds().width / 3.f), 10.f + (font.getLineSpacing(32) * 5));

		// Adds a callback for the window resized event
		Window::GetWindowResizedEvent().AddCallback(&Simulation::OnWindowResized, *this);
	}

	// Updates the simulation screen (includes drawing to window if required)
	void Simulation::Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw)
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Sets the window view to the simulation view
		window.setView(m_simulationView);

		// Draws the track if needed
		if (draw)
			RaceTrack::Update();

		// Sets the best car to the front of the cars list
		Machine::Car* bestCar = cars[0];

		// Iterates through the cars in the simulation
		for (Machine::Car* car : cars)
		{
			// Checks whether the car is alive and updates it if it is
			// Decrements alive count if the car dies this frame
			if (car->IsAlive() && car->Update())
				aliveCount--;

			// Checks to see if the cars fitness is greater than the best cars fitness
			// and sets it to the best car if it is
			if (car->CalcFitness() > bestCar->GetFitness())
				bestCar = car;

			// Draws the car to the window if needed
			if (draw)
				window.draw(*car);
		}

		// Checks to see if the new best car is different from the best car in the previous frame
		if (m_prevBestCar != bestCar)
		{
			// Sets the previous best car to a normal car (if the car exists)
			if (m_prevBestCar)
				m_prevBestCar->SetBestCar(false);
			// Sets the new best car to best car if it is alive
			if (bestCar->IsAlive())
				bestCar->SetBestCar(true);
			// Sets the previous best car to the new best car
			m_prevBestCar = bestCar;
		}
		
		// Draws the UI if needed
		if (draw)
		{
			// Moves the view to the best cars position
			m_simulationView.move((bestCar->GetPos() - m_simulationView.getCenter()) / 50.f);

			// Sets the window view to the UI view and draws the UI
			window.setView(m_uiView);
			window.draw(m_seedText);
			window.draw(m_iterationText);
			window.draw(m_showCheckpointsText);
			window.draw(m_showCheckpointsCB);

			// Sets the window view to the network view and draws the best cars' neural network
			window.setView(m_networkView);
			window.draw(bestCar->GetNeuralNetwork());
		}

	}

	// Loads the simulation screen
	void Simulation::Load()
	{
		m_showCheckpointsCB.SetActive(true);
	}

	// Unloads the simulation screen
	void Simulation::Unload()
	{
		m_showCheckpointsCB.SetActive(false);
	}

	// Sets the seed text for the simulation screen
	void Simulation::SetSeedText(unsigned int seed)
	{
		m_seedText.setString("Seed: " + std::to_string(seed));
	}

	// Sets the iteration text for the simulation screen and resets the simulation view position
	void Simulation::SetIteration(unsigned int iteration)
	{
		m_iterationText.setString("Generation: " + std::to_string(iteration));
		m_simulationView.setCenter(RaceTrack::GetStartPos());
	}
}