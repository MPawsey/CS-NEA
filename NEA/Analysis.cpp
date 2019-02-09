#include "Analysis.h"
#include <vector>
#include "UI.h"
#include "Window.h"
#include "EvolutionManager.h"
#include <array>
#include "LineShape.h"

namespace Evolution::Analysis
{
	// Private
	std::vector<sf::Vertex> m_fitnessMax, m_fitnessAvg, m_fitnessMin;
	std::vector<std::array<sf::Vertex, 2>> m_graphGuideLines;
	UI::Button m_nextBtn, m_next10Btn, m_nextQuickBtn, m_next10QuickBtn, m_saveBtn, m_menuBtn;
	size_t m_size = 0;
	sf::View m_analysisView, m_graphView;
	float m_graphMin = 0.f, m_graphMax = 0.f;
	float m_graphGuideSeperation = 100.f;

	void OnWindowClose()
	{
		m_nextBtn.UninitialiseEvents();
		m_next10Btn.UninitialiseEvents();
		m_nextQuickBtn.UninitialiseEvents();
		m_next10QuickBtn.UninitialiseEvents();
		m_saveBtn.UninitialiseEvents();
		m_menuBtn.UninitialiseEvents();
	}

	// Public

	void Init()
	{
		Window::GetWindowClosedEvent().AddCallback(OnWindowClose);

		m_graphView.setViewport(sf::FloatRect{ 0.05f, 0.45f, 0.75f, 0.5f });
		m_analysisView = Window::GetDefaultWindowView();

		std::array<sf::Vertex, 2> base = { sf::Vertex{ sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta }, sf::Vertex{sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta} };
		m_graphGuideLines.push_back(base);
		m_graphGuideLines.push_back(base);

		float xPos = 50.f, xGap = 250.f;
		float yPos1 = 50.f, yPos2 = 150.f;

		m_nextBtn = UI::Button{ "Next", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::StartNextGeneration(1, true); });
		m_nextBtn.setPosition(xPos, yPos1);
		m_nextQuickBtn = UI::Button{ "Next Quick", m_analysisView };
		m_nextQuickBtn.SetCentreText(true);
		m_nextQuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::StartNextGeneration(1, false); });
		m_nextQuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		m_next10Btn = UI::Button{ "Next 10", m_analysisView };
		m_next10Btn.SetCentreText(true);
		m_next10Btn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::StartNextGeneration(10, true); });
		m_next10Btn.setPosition(xPos, yPos1);
		m_next10QuickBtn = UI::Button{ "Next 10 Quick", m_analysisView };
		m_next10QuickBtn.SetCentreText(true);
		m_next10QuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::StartNextGeneration(10, false); });
		m_next10QuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		m_saveBtn = UI::Button{ "Save Cars", m_analysisView };
		m_saveBtn.SetCentreText(true);
		m_saveBtn.GetMouseClickedEvent().AddCallback([]() { std::cout << "SAVE CLICKED\n"; });
		m_saveBtn.setPosition(xPos, yPos1);
		m_menuBtn = UI::Button{ "Menu", m_analysisView };
		m_menuBtn.SetCentreText(true);
		m_menuBtn.GetMouseClickedEvent().AddCallback([]() { std::cout << "MENU CLICKED\n"; });
		m_menuBtn.setPosition(xPos, yPos2);
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		window.setView(m_analysisView);
		window.draw(m_nextBtn);
		window.draw(m_next10Btn);
		window.draw(m_nextQuickBtn);
		window.draw(m_next10QuickBtn);
		window.draw(m_saveBtn);
		window.draw(m_menuBtn);

		window.setView(m_graphView);
		window.draw(m_fitnessMin.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessAvg.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessMax.data(), m_size, sf::LineStrip);

		for (auto& line : m_graphGuideLines)
		{
			window.draw(line.data(), 2, sf::LinesStrip);
		}
	}

	void Load()
	{
		Window::GetWindow().setFramerateLimit(Window::MENU_FRAMERATE);

		m_nextBtn.SetActive(true);
		m_nextQuickBtn.SetActive(true);
		m_next10Btn.SetActive(true);
		m_next10QuickBtn.SetActive(true);
		m_saveBtn.SetActive(true);
		m_menuBtn.SetActive(true);
	}

	void Unload()
	{
		m_nextBtn.SetActive(false);
		m_nextQuickBtn.SetActive(false);
		m_next10Btn.SetActive(false);
		m_next10QuickBtn.SetActive(false);
		m_saveBtn.SetActive(false);
		m_menuBtn.SetActive(false);
	}

	void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin)
	{
		bool updateGuideLines = false;
		m_graphGuideLines[0][1].position.x = (float)++m_size;

		if (fitnessMin < m_graphMin)
		{
			m_graphMin = fitnessMin;
			m_graphGuideLines[1][0].position.y = m_graphMin;
			updateGuideLines = true;
		}
		if (fitnessMax > m_graphMax)
		{
			m_graphMax = fitnessMax;
			m_graphGuideLines[1][1].position.y = m_graphMax;
			updateGuideLines = true;
		}

		if (updateGuideLines)
		{
			std::array<std::array<sf::Vertex, 2>, 2> baseLines = { m_graphGuideLines[0], m_graphGuideLines[1] };
			m_graphGuideLines.clear();
			m_graphGuideLines.push_back(baseLines[0]);
			m_graphGuideLines.push_back(baseLines[1]);
			
			float height = abs(m_graphMax) + abs(m_graphMin);

			while (m_graphGuideSeperation * 10.f < height)
			{
				m_graphGuideSeperation += 100.f;
			}

			std::array<sf::Vertex, 2> line = { sf::Vertex{sf::Vector2f{0.f, 0.f}, sf::Color::White}, sf::Vertex{sf::Vector2f{(float)m_size, 0.f}, sf::Color::White} };

			for (int i = m_graphMin < 0 ? ceilf(m_graphMin / m_graphGuideSeperation) : floorf(m_graphMin / m_graphGuideSeperation); i < ceilf(m_graphMax / m_graphGuideSeperation); i++)
			{
				if (i == 0)
					continue;

				line[0].position.y = i * m_graphGuideSeperation;
				line[1].position.y = i * m_graphGuideSeperation;
				m_graphGuideLines.push_back(line);
			}
		}
		else
		{
			for (size_t i = 2; i < m_graphGuideLines.size(); i++)
			{
				m_graphGuideLines[i][1].position.x = (float)m_size;
			}
		}

		m_fitnessMax.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessMax }, sf::Color::Cyan });
		m_fitnessAvg.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessAvg }, sf::Color::Yellow });
		m_fitnessMin.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessMin }, sf::Color::Red });


		m_graphView.setSize(sf::Vector2f{ (float)m_size, -abs(m_graphMax) - abs(m_graphMin) });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, (m_graphMax + m_graphMin) / 2.f });
	}
}