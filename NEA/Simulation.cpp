#include "Simulation.h"
#include "Window.h"
#include "UI.h"
#include "RaceTrack.h"

namespace Evolution::Simulation
{
	sf::View m_uiView;
	sf::View m_networkView;
	sf::View m_simulationView;
	sf::Text m_seedText;
	sf::Text m_iterationText;
	Machine::Car* m_prevBestCar;

	void OnWindowResized(sf::Vector2u size)
	{
		m_simulationView.setSize((sf::Vector2f)size);
		m_uiView.setSize((sf::Vector2f)size);
	}

	void Init()
	{
		const sf::Font& font = UI::GetFont();

		m_uiView = m_simulationView = Window::GetWindow().getDefaultView();

		m_networkView.setSize(1.f, 1.f);
		m_networkView.setCenter(0.5f, 0.5f);
		m_networkView.setViewport(sf::FloatRect{ 0.7f, 0.f, 0.3f, 0.3f });

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
		

		Window::GetWindowResizedEvent().AddCallback(OnWindowResized);
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

		if (m_prevBestCar != bestCar)
		{
			if (m_prevBestCar)
				m_prevBestCar->SetBestCar(false);
			bestCar->SetBestCar(true);
			m_prevBestCar = bestCar;
		}
		
		m_simulationView.setCenter(bestCar->GetPos());

		window.setView(m_uiView);
		window.draw(m_seedText);
		window.draw(m_iterationText);

		window.setView(m_networkView);
		window.draw(bestCar->GetNeuralNetwork());
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