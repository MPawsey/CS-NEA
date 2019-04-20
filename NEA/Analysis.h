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

		std::vector<sf::Vertex> m_fitnessMax, m_fitnessAvg, m_fitnessMin;
		std::vector<std::array<sf::Vertex, 2>> m_graphGuideLines;
		std::array<sf::Vertex, 2> m_graphFinishLine;
		sf::VertexArray m_viewLine;
		UI::Button m_nextBtn, m_next10Btn, m_nextQuickBtn, m_next10QuickBtn, m_saveBtn, m_menuBtn;
		UI::Popup m_savePopup;
		size_t m_size = 0;
		sf::View m_analysisView, m_graphView;
		float m_graphMin = 0.f, m_graphMax = 0.f;
		float m_graphGuideSeperation = 100.f;
		sf::Text m_iterText, m_scaleText, m_fitnessText, m_bestText, m_avgText, m_worstText;
		bool m_isMouseDown = false;

		void SetAnalysisActive(bool isActive);
		void OnMousePressed();
		void OnMouseReleased();
	public:
		void Init();
		void Update();

		void Load();
		void Unload();
		void Reset();

		void SetFinishLine(float distance);
		void SetGraph(std::vector<float> positions);
		void UpdateGraph(float fitnessMax, float fitnessAvg, float fitnessMin);
		void SaveGraph(std::ofstream& file);
	};
}