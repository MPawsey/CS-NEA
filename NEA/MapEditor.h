#pragma once

#include <sfml/Graphics.hpp>
#include <vector>
#include <array>
#include "LineShape.h"
#include "CheckBox.h"
#include "Button.h"
#include "Popup.h"
#include "TextField.h"

namespace Editor
{
	enum MouseType
	{
		None,
		Point,
		Wall,
		InstantWall,
		Spawn,
		Checkpoint,
		Delete
	};

	struct MapCheckpoint
	{
		sf::CircleShape m_outline, m_centre;
		sf::Text m_idText;
	};

	struct Connection
	{
		std::array<sf::Vertex, 2> m_line;
		unsigned int pos1, pos2;

		Connection(sf::Vertex v1, sf::Vertex v2, unsigned int p1, unsigned int p2)
		{
			m_line = { v1, v2 };
			pos1 = p1;
			pos2 = p2;
		}

		friend bool operator<(const Connection& lhs, const Connection& rhs)
		{
			return lhs.pos1 < rhs.pos1 || (lhs.pos1 == rhs.pos1 && lhs.pos2 < rhs.pos2);
		}
	};

	class MapEditor
	{
	private:


		// Views
		sf::View m_gridView, m_editorView, m_UIView;

		// General stuff
		bool m_isActive = false;
		MouseType m_mouseType = MouseType::Point;
		int m_selectedPoint = -1;

		sf::CircleShape m_ghost;
		std::vector<sf::CircleShape> m_points;
		std::vector<Connection> m_connections;
		std::vector<MapCheckpoint> m_checkPoints;
		bool m_isDrawingCheckpoint = false;
		sf::RectangleShape m_spawnPos;
		LineShape m_spawnDir;
		bool m_spawnRotPressed = false;
		bool m_editorGrabbed = false;
		sf::Vector2i m_lastMousePos;

		// Grid
		bool m_gridVisible = true;
		bool m_gridLock = true;
		float m_gridSize = 50.f;
		std::vector<std::array<sf::Vertex, 2>> m_gridLines;

		// UI

		// Mouse types
		UI::CheckBox m_mtPointCB, m_mtWallCB, m_mtInstWallCB, m_mtSpawnCB, m_mtCheckpoint, m_mtDeleteCB;
		sf::Text m_mtPointText, m_mtWallText, m_mtInstWallText, m_mtSpawnText, m_mtCheckpointText, m_mtDeleteText;

		// Grid size
		UI::TextField m_gridSizeTF;
		UI::CheckBox m_gridVisibleCB, m_gridLockCB;
		sf::Text m_gridSizeText, m_gridVisibleText, m_gridLockText;

		// Save & Menu
		UI::Button m_saveBtn, m_menuBtn;
		UI::Popup m_savePop;

		void SaveMap(std::string filename);

		void RecalculateGrid();
		sf::Vector2f GetMousePointOnGrid();

		int FindLastPotentialConnection(unsigned int point);

		// Events
		void OnMousePressed(sf::Mouse::Button btn);
		void OnMouseReleased(sf::Mouse::Button btn);
		void OnMouseMoved(sf::Vector2i mousePos);
		void OnKeyPressed(sf::Keyboard::Key key);
		void OnWindowResized(sf::Vector2u size);

		void ChangeMouseType(MouseType type);

	public:


		static MapEditor& GetMapEditor();

		void Reset();

		void Init();
		void Update();
		void Load();
		void Unload();

	};
}
