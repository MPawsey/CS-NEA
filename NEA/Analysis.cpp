#include "Analysis.h"
#include <vector>
#include "Window.h"
#include "EvolutionManager.h"
#include "LineShape.h"
#include "MenuManager.h"
#include "InputManager.h"

namespace Evolution
{
	// Private

	void Analysis::SetAnalysisActive(bool isActive)
	{
		m_nextBtn.SetActive(isActive);
		m_next10Btn.SetActive(isActive);
		m_nextQuickBtn.SetActive(isActive);
		m_next10QuickBtn.SetActive(isActive);
		m_saveBtn.SetActive(isActive);
		m_menuBtn.SetActive(isActive);
	}

	void Analysis::OnMousePressed()
	{
		if (InputManager::IsMouseInView(m_graphView))
		{
			m_isMouseDown = true;
		}
	}

	void Analysis::OnMouseReleased()
	{
		m_isMouseDown = false;
	}

	// Public

	void Analysis::Init()
	{

		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Analysis::OnMousePressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Analysis::OnMouseReleased, *this);

		m_graphView.setViewport(sf::FloatRect{ 0.05f, 0.45f, 0.7f, 0.5f });
		m_analysisView = Window::GetDefaultWindowView();

		const sf::Font& font = UI::GetFont();

		m_scaleText.setFont(font);
		m_scaleText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 250.f);

		m_iterText.setFont(font);
		m_iterText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 280.f);

		m_fitnessText.setFont(font);
		m_fitnessText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 330.f);
		m_fitnessText.setString("Fitness");

		m_bestText.setFont(font);
		m_bestText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 360.f);
		m_bestText.setFillColor(sf::Color::Cyan);

		m_avgText.setFont(font);
		m_avgText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 390.f);
		m_avgText.setFillColor(sf::Color::Yellow);

		m_worstText.setFont(font);
		m_worstText.setPosition((Window::GetWindowSize().x * 0.75f) + 5.f, 420.f);
		m_worstText.setFillColor(sf::Color::Red);

		m_viewLine.resize(2);
		m_viewLine.setPrimitiveType(sf::PrimitiveType::LineStrip);
		m_viewLine[0] = sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::Green };
		m_viewLine[1] = sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::Green };

		m_savePopup = UI::Popup{ m_analysisView };
		m_savePopup.SetViewportTransform({0.25f, 0.25f, 0.5f, 0.5f});
		m_savePopup.SetViewSize((sf::Vector2f)Window::GetWindowSize() * 0.5f);

		UI::TextField* nameTF = new UI::TextField{ 300, UI::TextField::FieldType::Text, m_savePopup.GetView() };
		nameTF->SetTooltipText("The name of the save file.");
		nameTF->SetRawText("Cars");
		nameTF->setPosition((m_savePopup.GetView().getSize().x - nameTF->GetBounds().width) / 2.f, 50.f);
		m_savePopup.AddElement(nameTF); // No deleting of retBtn as m_savePopup destructor deletes it

		UI::Button* retBtn = new UI::Button{ "Cancel", m_savePopup.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		retBtn->setPosition(5.f, m_savePopup.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		retBtn->GetMouseClickedEvent().AddCallback([&]() { m_savePopup.SetActive(false); SetAnalysisActive(true); });
		retBtn->SetCentreText(true);
		m_savePopup.AddElement(retBtn); 

		UI::Button* saveBtn = new UI::Button{ "Save", m_savePopup.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		saveBtn->setPosition(m_savePopup.GetView().getSize().x - retBtn->GetBounds().width - 5.f, m_savePopup.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		saveBtn->GetMouseClickedEvent().AddCallback([&, nameTF]() { EvolutionManager::GetEvolutionManager().SaveGeneration(nameTF->GetRawText()); m_savePopup.SetActive(false); SetAnalysisActive(true); });
		saveBtn->SetCentreText(true);
		m_savePopup.AddElement(saveBtn);

		std::array<sf::Vertex, 2> base = { sf::Vertex{ sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta }, sf::Vertex{sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta} };
		m_graphGuideLines.push_back(base);
		m_graphGuideLines.push_back(base);

		float xPos = 50.f, xGap = 250.f;
		float yPos1 = 50.f, yPos2 = 150.f;

		m_nextBtn = UI::Button{ "Next", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(1, true); });
		m_nextBtn.setPosition(xPos, yPos1);
		m_nextQuickBtn = UI::Button{ "Next Quick", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_nextQuickBtn.SetCentreText(true);
		m_nextQuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(1, false); });
		m_nextQuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		m_next10Btn = UI::Button{ "Next 10", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_next10Btn.SetCentreText(true);
		m_next10Btn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(10, true); });
		m_next10Btn.setPosition(xPos, yPos1);
		m_next10QuickBtn = UI::Button{ "Next 10 Quick", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_next10QuickBtn.SetCentreText(true);
		m_next10QuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(10, false); });
		m_next10QuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		m_saveBtn = UI::Button{ "Save Cars", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_saveBtn.SetCentreText(true);
		m_saveBtn.GetMouseClickedEvent().AddCallback([&]() { m_savePopup.SetActive(true); SetAnalysisActive(false); });
		m_saveBtn.setPosition(xPos, yPos1);
		m_menuBtn = UI::Button{ "Menu", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_menuBtn.SetCentreText(true);
		m_menuBtn.GetMouseClickedEvent().AddCallback([]() { Window::SetWindowState(Window::Menu); Menu::MenuManager::GetMenuManager().GoToState(Menu::MenuState::MainMenu); });
		m_menuBtn.setPosition(xPos, yPos2);
	}

	void Analysis::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();


		if (m_isMouseDown && m_size > 0)
		{
			int generation = (int)std::round(std::clamp(std::round(InputManager::GetMousePosInView(m_graphView).x), 0.f, (float)m_size - 1));
			if (generation != m_viewLine[0].position.x)
			{
				if (generation == 0)
				{
					m_viewLine[0].position.x = 0.01f;
					m_viewLine[1].position.x = 0.01f;
				}
				else
				{
					m_viewLine[0].position.x = generation;
					m_viewLine[1].position.x = generation;
				}

				m_iterText.setString("Gen: " + std::to_string(generation));

				m_bestText.setString("Best: " + std::to_string((int)std::roundf(m_fitnessMax[generation].position.y)));
				m_avgText.setString("Avg: " + std::to_string((int)std::roundf(m_fitnessAvg[generation].position.y)));
				m_worstText.setString("Worst: " + std::to_string((int)std::roundf(m_fitnessMin[generation].position.y)));
			}
		}


		window.setView(m_analysisView);
		window.draw(m_nextBtn);
		window.draw(m_next10Btn);
		window.draw(m_nextQuickBtn);
		window.draw(m_next10QuickBtn);
		window.draw(m_saveBtn);
		window.draw(m_menuBtn);
		window.draw(m_iterText);
		window.draw(m_scaleText);
		window.draw(m_bestText);
		window.draw(m_avgText);
		window.draw(m_worstText);

		window.setView(m_graphView);

		for (auto& line : m_graphGuideLines)
		{
			window.draw(line.data(), 2, sf::LinesStrip);
		}

		window.draw(m_graphFinishLine.data(), 2, sf::LinesStrip);

		window.draw(m_fitnessMin.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessAvg.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessMax.data(), m_size, sf::LineStrip);

		window.draw(m_viewLine);

		window.draw(m_savePopup);
	}

	void Analysis::Load()
	{
		Window::GetWindow().setFramerateLimit(Window::MENU_FRAMERATE);

		SetAnalysisActive(true);
	}

	void Analysis::Unload()
	{
		SetAnalysisActive(false);
	}

	void Analysis::Reset()
	{
		m_fitnessMax.clear();
		m_fitnessAvg.clear();
		m_fitnessMin.clear();

		m_size = 0;
		m_graphMin = 0.f; 
		m_graphMax = 0.f;
		m_graphGuideSeperation = 100.f;
	}

	void Analysis::SetFinishLine(float distance)
	{
		m_graphFinishLine[0] = sf::Vertex{ sf::Vector2f{0, distance}, sf::Color::Green };
		m_graphFinishLine[1] = sf::Vertex{ sf::Vector2f{(float)m_size, distance}, sf::Color::Green };
	}

	void Analysis::SetGraph(std::vector<float> positions)
	{
		Reset();
		m_size = positions.size() / 3;
		m_graphFinishLine[1].position.x = (float)m_size;

		for (unsigned int i = 0; i < m_size; i++)
		{
			if (positions[i + (m_size * 2)] < m_graphMin)
			{
				m_graphMin = positions[i + (m_size * 2)];
				m_graphGuideLines[1][0].position.y = m_graphMin;
			}
			if (positions[i] > m_graphMax)
			{
				m_graphMax = positions[i];
				m_graphGuideLines[1][1].position.y = m_graphMax;
			}

			m_fitnessMax.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i]}, sf::Color::Cyan });
			m_fitnessAvg.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i + m_size]}, sf::Color::Yellow });
			m_fitnessMin.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i + (m_size * 2)]}, sf::Color::Red });
		}

		std::array<std::array<sf::Vertex, 2>, 2> baseLines = { m_graphGuideLines[0], m_graphGuideLines[1] };
		m_graphGuideLines.clear();
		m_graphGuideLines.push_back(baseLines[0]);
		m_graphGuideLines.push_back(baseLines[1]);
		m_graphGuideLines[0][1].position.x = m_size;

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


		m_graphView.setSize(sf::Vector2f{ (float)m_size, -abs(m_graphMax) - abs(m_graphMin) - 2.f });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, ((m_graphMax + m_graphMin) / 2.f) });

		if (m_size > 0)
		{
			m_iterText.setString("Gen: " + std::to_string(m_size - 1));
			m_bestText.setString("Best: " + std::to_string((int)std::roundf(m_fitnessMax.back().position.y)));
			m_avgText.setString("Avg: " + std::to_string((int)std::roundf(m_fitnessAvg.back().position.y)));
			m_worstText.setString("Worst: " + std::to_string((int)std::roundf(m_fitnessMin.back().position.y)));
		}
		else
		{
			m_iterText.setString("Gen: ");
			m_bestText.setString("Best: ");
			m_avgText.setString("Avg: ");
			m_worstText.setString("Worst: ");
		}
		m_scaleText.setString("Y Scale: " + std::to_string((int)std::round(m_graphGuideSeperation)));


		m_viewLine[0].position = sf::Vector2f{ (float)m_size - 1, m_graphMax };
		m_viewLine[1].position = sf::Vector2f{ (float)m_size - 1, m_graphMin };
	}

	void Analysis::UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin)
	{
		bool updateGuideLines = false;
		m_graphGuideLines[0][1].position.x = (float)++m_size;
		m_graphFinishLine[1].position.x = (float)m_size;

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


		m_graphView.setSize(sf::Vector2f{ (float)m_size, -abs(m_graphMax) - abs(m_graphMin) - 1.f });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, (m_graphMax + m_graphMin) / 2.f });

		m_iterText.setString("Gen: " + std::to_string(m_size - 1));
		m_scaleText.setString("Y Scale: " + std::to_string((int)std::round(m_graphGuideSeperation)));

		m_bestText.setString("Best: " + std::to_string((int)std::roundf(m_fitnessMax.back().position.y)));
		m_avgText.setString("Avg: " + std::to_string((int)std::roundf(m_fitnessAvg.back().position.y)));
		m_worstText.setString("Worst: " + std::to_string((int)std::roundf(m_fitnessMin.back().position.y)));

		m_viewLine[0].position = sf::Vector2f{ (float)m_size - 1, m_graphMax };
		m_viewLine[1].position = sf::Vector2f{ (float)m_size - 1, m_graphMin };
	}

	void Analysis::SaveGraph(std::ofstream& file)
	{
		file << 'g';
		for (auto point : m_fitnessMax)
		{
			file << " " << point.position.y;
		}
		for (auto point : m_fitnessAvg)
		{
			file << " " << point.position.y;
		}
		for (auto point : m_fitnessMin)
		{
			file << " " << point.position.y;
		}
		file << std::endl;
	}
}