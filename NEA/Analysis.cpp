#include "Analysis.h"
#include <vector>
#include "UI.h"
#include "Window.h"
#include "EvolutionManager.h"

namespace Evolution::Analysis
{
	// Private
	std::vector<sf::Vertex> m_fitnessMax, m_fitnessAvg, m_fitnessMin;
	UI::Button m_nextBtn;// , m_saveBtn, m_menuBtn;
	size_t m_size = 0;
	sf::View m_analysisView, m_graphView;
	float m_graphMin, m_graphMax;

	// Public

	void Init()
	{
		m_graphView.setViewport(sf::FloatRect{ 0.f, 0.5f, 0.5f, 0.5f });
		m_analysisView = Window::GetDefaultWindowView();
		//m_nextBtn = UI::Button{"Next Generation", m_analysisView};
		//m_nextBtn.SetCentreText(true);
		//m_nextBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::StartNextGeneration(); });
		//m_nextBtn.setPosition(m_analysisView.getSize().x - m_nextBtn.GetBounds().width - 5.f, m_analysisView.getSize().y - m_nextBtn.GetBounds().height - 5.f);
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_analysisView);
		//window.draw(m_nextBtn);


		window.setView(m_graphView);
		window.draw(m_fitnessMin.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessAvg.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessMax.data(), m_size, sf::LineStrip);

	}

	void Load()
	{
		//m_nextBtn.SetActive(true);
	}

	void Unload()
	{
		//m_nextBtn.SetActive(false);
	}

	void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin)
	{
		if (fitnessMin < m_graphMin)
			m_graphMin = fitnessMin;
		if (fitnessMax < m_graphMax)
			m_graphMax = fitnessMax;

		m_fitnessMax.push_back(sf::Vertex{ sf::Vector2f{ (float)++m_size, fitnessMax }, sf::Color::Cyan });
		m_fitnessAvg.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size, fitnessMax }, sf::Color::Yellow });
		m_fitnessMin.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size, fitnessMax }, sf::Color::Red });

		m_graphView.setSize(sf::Vector2f{ (float)m_size, abs(m_graphMax) + abs(m_graphMin) });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, (m_graphMax + m_graphMin) / 2.f });


	}
}