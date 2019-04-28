#include "Analysis.h"
#include <vector>
#include "Window.h"
#include "EvolutionManager.h"
#include "LineShape.h"
#include "MenuManager.h"
#include "InputManager.h"

namespace Evolution
{
	// Sets the activity state of the UI to isActive
	void Analysis::SetAnalysisActive(bool isActive)
	{
		m_nextBtn.SetActive(isActive);
		m_next10Btn.SetActive(isActive);
		m_nextQuickBtn.SetActive(isActive);
		m_next10QuickBtn.SetActive(isActive);
		m_saveBtn.SetActive(isActive);
		m_menuBtn.SetActive(isActive);
	}

	// Called when the mouse is pressed
	void Analysis::OnMousePressed()
	{
		// Sets m_isMouseDown to true if the mouse is currently inside of the graph view
		if (InputManager::IsMouseInView(m_graphView))
		{
			m_isMouseDown = true;
		}
	}

	// Sets m_isMouseDown to false whenever the mouse is released
	void Analysis::OnMouseReleased()
	{
		m_isMouseDown = false;
	}

	// Initialises the values of the analysis screen
	void Analysis::Init()
	{
		// Gets the fonr for the UI
		const sf::Font& font = UI::GetFont();

		// Adds a callback for the left mouse button pressed and released events
		InputManager::GetMousePressedEvent(sf::Mouse::Left).AddCallback(&Analysis::OnMousePressed, *this);
		InputManager::GetMouseReleasedEvent(sf::Mouse::Left).AddCallback(&Analysis::OnMouseReleased, *this);

		// Initialises the views
		m_graphView.setViewport(sf::FloatRect{ 0.05f, 0.45f, 0.7f, 0.5f });
		m_analysisView = Window::GetDefaultWindowView();

		// Initialises the text for the graph
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

		// Initialises the line that shows what generation the graph is currently looking at
		m_viewLine.resize(2);
		m_viewLine.setPrimitiveType(sf::PrimitiveType::LineStrip);
		m_viewLine[0] = sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::Green };
		m_viewLine[1] = sf::Vertex{ sf::Vector2f{0.f, 0.f}, sf::Color::Green };

		// Initialises the save popup
		m_savePop = UI::UIPopup{ m_analysisView };
		m_savePop.SetViewportTransform({0.25f, 0.25f, 0.5f, 0.5f});
		m_savePop.SetViewSize((sf::Vector2f)Window::GetWindowSize() * 0.5f);

		// Initialises and adds the name text field to the save popup
		UI::UITextField* nameTF = new UI::UITextField{ 300, UI::UITextField::FieldType::Text, m_savePop.GetView() };
		nameTF->SetTooltipText("The name of the save file.");
		nameTF->SetRawText("Cars");
		nameTF->setPosition((m_savePop.GetView().getSize().x - nameTF->GetBounds().width) / 2.f, 50.f);
		m_savePop.AddElement(nameTF); // No deleting of nameTF as m_savePopup destructor deletes it

		// Initialises and adds the return button to the save popup
		UI::UIButton* retBtn = new UI::UIButton{ "Cancel", m_savePop.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		retBtn->setPosition(5.f, m_savePop.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		retBtn->GetMouseClickedEvent().AddCallback([&]() { m_savePop.SetActive(false); SetAnalysisActive(true); });
		retBtn->SetCentreText(true);
		m_savePop.AddElement(retBtn); 

		// Initialises and adds the save button to the save popup
		UI::UIButton* saveBtn = new UI::UIButton{ "Save", m_savePop.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		saveBtn->setPosition(m_savePop.GetView().getSize().x - retBtn->GetBounds().width - 5.f, m_savePop.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		saveBtn->GetMouseClickedEvent().AddCallback([&, nameTF]() { EvolutionManager::GetEvolutionManager().SaveGeneration(nameTF->GetRawText()); m_savePop.SetActive(false); SetAnalysisActive(true); });
		saveBtn->SetCentreText(true);
		m_savePop.AddElement(saveBtn);

		// Creates and adds the axis lines to the graph guide lines
		std::array<sf::Vertex, 2> base = { sf::Vertex{ sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta }, sf::Vertex{sf::Vector2f{0.01f, 0.01f}, sf::Color::Magenta} };
		m_graphGuideLines.push_back(base);
		m_graphGuideLines.push_back(base);

		// Sets positions for UI
		float xPos = 50.f, xGap = 250.f;
		float yPos1 = 50.f, yPos2 = 150.f;

		// UI increments the pos values where needed
		// between the initialisation of the buttons

		// Initialises the next and next quick buttons
		m_nextBtn = UI::UIButton{ "Next", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_nextBtn.SetCentreText(true);
		m_nextBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(1, true); });
		m_nextBtn.setPosition(xPos, yPos1);
		m_nextQuickBtn = UI::UIButton{ "Next Quick", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_nextQuickBtn.SetCentreText(true);
		m_nextQuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(1, false); });
		m_nextQuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		// Initialises the next 10 and next 10 quick buttons
		m_next10Btn = UI::UIButton{ "Next 10", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_next10Btn.SetCentreText(true);
		m_next10Btn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(10, true); });
		m_next10Btn.setPosition(xPos, yPos1);
		m_next10QuickBtn = UI::UIButton{ "Next 10 Quick", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_next10QuickBtn.SetCentreText(true);
		m_next10QuickBtn.GetMouseClickedEvent().AddCallback([]() { EvolutionManager::GetEvolutionManager().StartNextGeneration(10, false); });
		m_next10QuickBtn.setPosition(xPos, yPos2);

		xPos += xGap;

		// Initialises the save and menu buttons
		m_saveBtn = UI::UIButton{ "Save Cars", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_saveBtn.SetCentreText(true);
		m_saveBtn.GetMouseClickedEvent().AddCallback([&]() { m_savePop.SetActive(true); SetAnalysisActive(false); });
		m_saveBtn.setPosition(xPos, yPos1);
		m_menuBtn = UI::UIButton{ "Menu", m_analysisView, UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		m_menuBtn.SetCentreText(true);
		m_menuBtn.GetMouseClickedEvent().AddCallback([this]() { SetAnalysisActive(false); EvolutionManager::GetEvolutionManager().Reset(); Window::SetWindowState(Window::Menu); Menu::MenuManager::GetMenuManager().GoToState(Menu::MenuState::MainMenu); });
		m_menuBtn.setPosition(xPos, yPos2);
	}

	// Updates the analysis screen
	// Includes the drawing to the window
	void Analysis::Update()
	{
		// Gets the window instance of the program
		sf::RenderWindow& window = Window::GetWindow();

		// Checks whether the user is moving the view line in the graph
		// Can only be done if the graph contains data
		if (m_isMouseDown && m_size > 0)
		{
			// Gets the generation the user is trying to inspect
			int generation = (int)std::round(std::clamp(std::round(InputManager::GetMousePosInView(m_graphView).x), 0.f, (float)m_size - 1));
			// Checks to see if anything needs to be updated
			if (generation != (int)m_viewLine[0].position.x)
			{
				// Sets the x position of the view line to the generation
				// Special case for gen = 0
				// Sets to 0.01f so that it is rendered correctly
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

				// Updates the info text for the inspected generation
				m_iterText.setString("Gen: " + std::to_string(generation));
				m_bestText.setString("Best: " + std::to_string((int)std::roundf(m_fitnessMax[generation].position.y)));
				m_avgText.setString("Avg: " + std::to_string((int)std::roundf(m_fitnessAvg[generation].position.y)));
				m_worstText.setString("Worst: " + std::to_string((int)std::roundf(m_fitnessMin[generation].position.y)));
			}
		}

		// Sets the view to the analysis view and draws all necessary UI
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

		// Sets the view to the graph view
		window.setView(m_graphView);

		// Draws the guid lines
		for (auto& line : m_graphGuideLines)
		{
			window.draw(line.data(), 2, sf::LinesStrip);
		}

		// Draws the finish distance line
		window.draw(m_graphFinishLine.data(), 2, sf::LinesStrip);

		// Draws the fitnesses to ontop of the graph
		window.draw(m_fitnessMin.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessAvg.data(), m_size, sf::LineStrip);
		window.draw(m_fitnessMax.data(), m_size, sf::LineStrip);

		// Draws the view line
		window.draw(m_viewLine);

		// Drwas the save popup
		window.draw(m_savePop);
	}

	// Loads the analysis screen
	void Analysis::Load()
	{
		// Sets the framerate to the menu framerate
		Window::GetWindow().setFramerateLimit(Window::MENU_FRAMERATE);

		// Sets the UI to be active
		SetAnalysisActive(true);
	}

	// Unloads the analysis screen
	void Analysis::Unload()
	{
		// Sets the UI to be unactive
		SetAnalysisActive(false);
	}

	// Resets the analysis screen
	void Analysis::Reset()
	{
		// Clears the fitness values for the graph
		m_fitnessMax.clear();
		m_fitnessAvg.clear();
		m_fitnessMin.clear();

		// Reinitialises all necessary values
		m_size = 0;
		m_graphMin = 0.f; 
		m_graphMax = 0.f;
		m_graphGuideSeperation = 100.f;
	}

	// Sets the finish line given a distance
	void Analysis::SetFinishLine(float distance)
	{
		m_graphFinishLine[0] = sf::Vertex{ sf::Vector2f{0, distance}, sf::Color::Green };
		m_graphFinishLine[1] = sf::Vertex{ sf::Vector2f{(float)m_size, distance}, sf::Color::Green };
	}

	// Sets the graph values given a vector of positions
	// positions works such that the vector is is split into thirds
	// The first third is the max
	// The second third is the average
	// The final third is the minimum
	void Analysis::SetGraph(std::vector<float> positions)
	{
		// Resets the screen
		Reset();

		// Gets the size of the graph and sets the finish line x position
		m_size = positions.size() / 3;
		m_graphFinishLine[1].position.x = (float)m_size;

		// Adds the values from the position vector to their respective places in the fitness vectors
		for (unsigned int i = 0; i < m_size; i++)
		{
			// Checks whether a new max/min has been found
			if (positions[i + (m_size * 2)] < m_graphMin)
			{
				// Sets the graph minimum to the new minimum
				m_graphMin = positions[i + (m_size * 2)];
				m_graphGuideLines[1][0].position.y = m_graphMin;
			}
			if (positions[i] > m_graphMax)
			{
				// Sets the graph maximum to the new maximum
				m_graphMax = positions[i];
				m_graphGuideLines[1][1].position.y = m_graphMax;
			}

			// Adds the fitness values from positions to their respective fitness vectors
			m_fitnessMax.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i]}, sf::Color::Cyan });
			m_fitnessAvg.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i + m_size]}, sf::Color::Yellow });
			m_fitnessMin.push_back(sf::Vertex{ sf::Vector2f{(float)i, positions[i + (m_size * 2)]}, sf::Color::Red });
		}

		// Clears the graph guide lines
		// Ensures the axis lines aren't removed
		std::array<std::array<sf::Vertex, 2>, 2> baseLines = { m_graphGuideLines[0], m_graphGuideLines[1] };
		m_graphGuideLines.clear();
		m_graphGuideLines.push_back(baseLines[0]);
		m_graphGuideLines.push_back(baseLines[1]);
		// Updates the right x position of the x axis
		m_graphGuideLines[0][1].position.x = m_size;

		// Finds the absolute height
		float height = abs(m_graphMax) + abs(m_graphMin);

		// Finds a suitable seperation size for the graph guide lines
		while (m_graphGuideSeperation * 10.f < height)
		{
			m_graphGuideSeperation += 100.f;
		}

		// Creates a template guide line
		std::array<sf::Vertex, 2> line = { sf::Vertex{sf::Vector2f{0.f, 0.f}, sf::Color::White}, sf::Vertex{sf::Vector2f{(float)m_size, 0.f}, sf::Color::White} };

		// Adds guide lines to the graph
		for (int i = m_graphMin < 0 ? ceilf(m_graphMin / m_graphGuideSeperation) : floorf(m_graphMin / m_graphGuideSeperation); i < ceilf(m_graphMax / m_graphGuideSeperation); i++)
		{
			// Skips on the x axis
			if (i == 0)
				continue;

			// Sets the y position of the line and adds it to the graph guide lines vector
			line[0].position.y = i * m_graphGuideSeperation;
			line[1].position.y = i * m_graphGuideSeperation;
			m_graphGuideLines.push_back(line);
		}

		// Reinitialises the graph view
		m_graphView.setSize(sf::Vector2f{ (float)m_size, -abs(m_graphMax) - abs(m_graphMin) - 2.f });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, ((m_graphMax + m_graphMin) / 2.f) });

		// Sets the info text of the graph
		// Special case for when m_size is 0 so that an error does not occur
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

		// Sets the view line to be the furthest right
		m_viewLine[0].position = sf::Vector2f{ (float)m_size - 1, m_graphMax };
		m_viewLine[1].position = sf::Vector2f{ (float)m_size - 1, m_graphMin };
	}

	// Appends a new set of fitness values to the graph and updates it
	void Analysis::UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin)
	{
		// Bool for whether the guide lines need to be updated
		bool updateGuideLines = false;
		
		// Updates the x position for the x axis and finish line
		m_graphGuideLines[0][1].position.x = (float)++m_size;
		m_graphFinishLine[1].position.x = (float)m_size;

		// Checks whether a new min/max has been found
		// and sets the graph min/max to the new one (if required)
		// Also sets updateGuideLines to true if required
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

		// Updates the guide lines if required
		if (updateGuideLines)
		{
			// Gets the base lines of the graph
			std::array<std::array<sf::Vertex, 2>, 2> baseLines = { m_graphGuideLines[0], m_graphGuideLines[1] };

			// Clears the guide lines and readds the base lines
			m_graphGuideLines.clear();
			m_graphGuideLines.push_back(baseLines[0]);
			m_graphGuideLines.push_back(baseLines[1]);
			
			// Finds the absolute height of the graph
			float height = abs(m_graphMax) + abs(m_graphMin);

			// Gets the appropriate vertical seperation for the guide lines (if required)
			while (m_graphGuideSeperation * 10.f < height)
			{
				m_graphGuideSeperation += 100.f;
			}

			// Creates a template guide line
			std::array<sf::Vertex, 2> line = { sf::Vertex{sf::Vector2f{0.f, 0.f}, sf::Color::White}, sf::Vertex{sf::Vector2f{(float)m_size, 0.f}, sf::Color::White} };

			// Adds guide lines to the graph
			for (int i = m_graphMin < 0 ? ceilf(m_graphMin / m_graphGuideSeperation) : floorf(m_graphMin / m_graphGuideSeperation); i < ceilf(m_graphMax / m_graphGuideSeperation); i++)
			{
				// Makes sure not to add a guide line where the axis line is
				if (i == 0)
					continue;

				// Sets the y value of the line guide line before adding it
				line[0].position.y = i * m_graphGuideSeperation;
				line[1].position.y = i * m_graphGuideSeperation;
				m_graphGuideLines.push_back(line);
			}
		}
		else
		{
			// Updates the x values of the guide lines if a full update is not needed
			for (size_t i = 2; i < m_graphGuideLines.size(); i++)
			{
				m_graphGuideLines[i][1].position.x = (float)m_size;
			}
		}

		// Adds the new fitness values to their respective vectors
		m_fitnessMax.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessMax }, sf::Color::Cyan });
		m_fitnessAvg.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessAvg }, sf::Color::Yellow });
		m_fitnessMin.push_back(sf::Vertex{ sf::Vector2f{ (float)m_size - 1, fitnessMin }, sf::Color::Red });

		// Updates the graph view
		m_graphView.setSize(sf::Vector2f{ (float)m_size, -abs(m_graphMax) - abs(m_graphMin) - 1.f });
		m_graphView.setCenter(sf::Vector2f{ (float)m_size / 2.f, (m_graphMax + m_graphMin) / 2.f });

		// Updates the graph info text
		m_iterText.setString("Gen: " + std::to_string(m_size - 1));
		m_scaleText.setString("Y Scale: " + std::to_string((int)std::roundf(m_graphGuideSeperation)));

		m_bestText.setString("Best: " + std::to_string((int)std::roundf(m_fitnessMax.back().position.y)));
		m_avgText.setString("Avg: " + std::to_string((int)std::roundf(m_fitnessAvg.back().position.y)));
		m_worstText.setString("Worst: " + std::to_string((int)std::roundf(m_fitnessMin.back().position.y)));

		// Sets the view line position to the furthest right
		m_viewLine[0].position = sf::Vector2f{ (float)m_size - 1, m_graphMax };
		m_viewLine[1].position = sf::Vector2f{ (float)m_size - 1, m_graphMin };
	}

	// Saves the graph to the output file stream in the format for .cars file
	void Analysis::SaveGraph(std::ofstream& file)
	{
		// Adds a g character to the beginning of the line
		file << 'g';

		// Adds the max fitness values to the line seperated by a space
		for (auto point : m_fitnessMax)
		{
			file << " " << point.position.y;
		}

		// Adds the average fitness values to the line seperated by a space
		for (auto point : m_fitnessAvg)
		{
			file << " " << point.position.y;
		}

		// Adds the min fitness values to the line seperated by a space
		for (auto point : m_fitnessMin)
		{
			file << " " << point.position.y;
		}
		
		// Adds end line characters to the line
		file << std::endl;
	}
}