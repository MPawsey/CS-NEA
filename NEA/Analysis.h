#pragma once

#include <sfml/Graphics.hpp>
#include <fstream>
#include <vector>
#include "Button.h"
#include "TextField.h"
#include <array>
#include "Popup.h"

namespace Evolution
{
	class Analysis
	{
	private:

		// The graph
		std::vector<sf::Vertex> m_fitnessMax, m_fitnessAvg, m_fitnessMin;
		std::vector<std::array<sf::Vertex, 2>> m_graphGuideLines;
		std::array<sf::Vertex, 2> m_graphFinishLine;
		float m_graphMin = 0.f, m_graphMax = 0.f;
		float m_graphGuideSeperation = 100.f;
		sf::VertexArray m_viewLine;
		size_t m_size = 0;

		// UI
		sf::Text m_iterText, m_scaleText, m_fitnessText, m_bestText, m_avgText, m_worstText;
		UI::UIButton m_nextBtn, m_next10Btn, m_nextQuickBtn, m_next10QuickBtn, m_saveBtn, m_menuBtn;
		UI::UIPopup m_savePop;

		// Other
		sf::View m_analysisView, m_graphView;
		bool m_isMouseDown = false;

		// Sets whether the buttons should be clickable
		void SetAnalysisActive(bool isActive);
		
		// Events
		void OnMousePressed();
		void OnMouseReleased();
	public:

		// Initialises all variables
		void Init();

		// Updates the analysis screen
		// Includes drawing to the window
		void Update();

		// Loads the screen
		void Load();

		// Unloads the screen
		void Unload();
		
		// Resets the variables back to their initial states
		void Reset();

		// Sets the finish line location
		void SetFinishLine(float distance);

		// Sets the pooints on the graph
		// The vector works such the order follows:
		// Max -> Avg -> Min
		void SetGraph(std::vector<float> positions);
		
		// Appends a new set of values to the graph
		void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin);

		// Saves the graph to the file stream using the format for .cars files
		void SaveGraph(std::ofstream& file);
	};
}