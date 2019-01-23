#include "Simulation.h"
#include "Window.h"
#include "UI.h"
#include "RaceTrack.h"

namespace Evolution::Simulation
{
	sf::View m_uiView;
	sf::View m_simulationView;
	sf::Text m_seedText;
	sf::Text m_iterationText;

	void Init()
	{
		const sf::Font& font = UI::GetFont();

		
		m_uiView = m_simulationView = Window::GetWindow().getDefaultView();
		m_simulationView.setSize((sf::Vector2f)Window::GetWindowSize());

		m_seedText.setFillColor(sf::Color::White);
		m_seedText.setOutlineColor(sf::Color::Black);
		m_seedText.setOutlineThickness(2.f);
		m_seedText.setFont(font);
		m_seedText.setPosition(10.f, 10.f);

		m_iterationText.setFillColor(sf::Color::White);
		m_iterationText.setOutlineColor(sf::Color::Black);
		m_iterationText.setOutlineThickness(2.f);
		m_iterationText.setFont(font);
		m_iterationText.setPosition(10.f, 10.f + font.getLineSpacing(32));

	}

	void Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount)
	{
		sf::RenderWindow& window = Window::GetWindow();
		window.setView(m_simulationView);
		RaceTrack::Update();

		Machine::Car* bestCar = cars[0];
		for (Machine::Car* car : cars)
		{
			if (car->IsAlive() && car->Update())
				aliveCount--;

			if (car->CalcFitness() > bestCar->GetFitness())
				bestCar = car;

			window.draw(*car);
		}

		m_simulationView.setCenter(bestCar->GetPos());

		window.setView(m_uiView);
		window.draw(m_seedText);
		window.draw(m_iterationText);
	}


	void SetSeedText(unsigned int seed)
	{
		m_seedText.setString("Seed: " + std::to_string(seed));
	}

	void SetIteration(unsigned int iteration)
	{
		m_iterationText.setString("Generation: " + std::to_string(iteration + 1));
	}
}