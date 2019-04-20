#include "Simulation.h"
#include "Window.h"
#include "UI.h"
#include "RaceTrack.h"

namespace Evolution
{

	void Simulation::OnWindowResized(sf::Vector2u size)
	{
		m_simulationView.setSize((sf::Vector2f)size);
		m_uiView.setSize((sf::Vector2f)size);
		m_uiView.setCenter((sf::Vector2f)size / 2.f);
	}

	void Simulation::Init()
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

		m_showCheckpointsText = sf::Text{ "    Show\nCheckpoints:", font };
		m_showCheckpointsText.setFillColor(sf::Color::White);
		m_showCheckpointsText.setOutlineColor(sf::Color::Black);
		m_showCheckpointsText.setOutlineThickness(2.f);
		m_showCheckpointsText.setPosition(10.f, 10.f + (font.getLineSpacing(32) * 3));

		m_showCheckpointsCB = UI::CheckBox{ m_uiView };
		m_showCheckpointsCB.SetChecked(true);
		m_showCheckpointsCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { RaceTrack::SetCheckpointsVisible(checked); });
		m_showCheckpointsCB.setPosition(m_showCheckpointsText.getGlobalBounds().left + (m_showCheckpointsText.getGlobalBounds().width / 3.f), 10.f + (font.getLineSpacing(32) * 5));

		Window::GetWindowResizedEvent().AddCallback(&Simulation::OnWindowResized, *this);
	}

	void Simulation::Update(std::vector<Machine::Car*>& cars, unsigned int& aliveCount, bool draw)
	{
		sf::RenderWindow& window = Window::GetWindow();
		window.setView(m_simulationView);

		if (draw)
			RaceTrack::Update();

		Machine::Car* bestCar = cars[0];
		for (Machine::Car* car : cars)
		{
			if (car->IsAlive() && car->Update())
				aliveCount--;

			if (car->CalcFitness() > bestCar->GetFitness())
				bestCar = car;

			if (draw)
				window.draw(*car);
		}

		if (m_prevBestCar != bestCar)
		{
			if (m_prevBestCar)
				m_prevBestCar->SetBestCar(false);
			if (bestCar->IsAlive())
				bestCar->SetBestCar(true);
			m_prevBestCar = bestCar;
		}
		

		if (draw)
		{
			m_simulationView.move((bestCar->GetPos() - m_simulationView.getCenter()) / 50.f);

			window.setView(m_uiView);
			window.draw(m_seedText);
			window.draw(m_iterationText);
			window.draw(m_showCheckpointsText);
			window.draw(m_showCheckpointsCB);

			window.setView(m_networkView);
			window.draw(bestCar->GetNeuralNetwork());
		}

	}

	void Simulation::Load()
	{
		m_showCheckpointsCB.SetActive(true);
	}

	void Simulation::Unload()
	{
		m_showCheckpointsCB.SetActive(false);
	}


	void Simulation::SetSeedText(unsigned int seed)
	{
		m_seedText.setString("Seed: " + std::to_string(seed));
	}

	void Simulation::SetIteration(unsigned int iteration)
	{
		m_iterationText.setString("Generation: " + std::to_string(iteration));
		m_simulationView.setCenter(RaceTrack::GetStartPos());
	}
}