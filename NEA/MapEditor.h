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
	// Different types of tools the mouse can utilise
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

	// Struct for a map checkpoint
	struct MapCheckpoint
	{
		sf::CircleShape m_outline, m_centre;
		sf::Text m_idText;
	};

	// Struct for the connection between two lines
	struct Connection
	{
		std::array<sf::Vertex, 2> m_line;
		unsigned int pos1, pos2;

		// Constructor for the connection
		Connection(sf::Vertex v1, sf::Vertex v2, unsigned int p1, unsigned int p2)
		{
			m_line = { v1, v2 };
			pos1 = p1;
			pos2 = p2;
		}

		// Less than boolean operator for two connections
		// Returns if the lhs has a lower pos1 id than the rhs (or pos2 id if pos1's are equal)
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

		// Mouse
		MouseType m_mouseType = MouseType::Point;
		sf::CircleShape m_ghost;
		bool m_isDrawingCheckpoint = false;
		bool m_spawnRotPressed = false;
		bool m_editorGrabbed = false;
		sf::Vector2i m_lastMousePos;

		// Map
		std::vector<sf::CircleShape> m_points;
		std::vector<Connection> m_connections;
		std::vector<MapCheckpoint> m_checkPoints;
		sf::RectangleShape m_spawnPos;
		LineShape m_spawnDir;

		// Other
		bool m_isActive = false;
		int m_selectedPoint = -1;

		// Grid
		bool m_gridVisible = true;
		bool m_gridLock = true;
		float m_gridSize = 50.f;
		std::vector<std::array<sf::Vertex, 2>> m_gridLines;

		// UI

		// Mouse types
		UI::UICheckBox m_mtPointCB, m_mtWallCB, m_mtInstWallCB, m_mtSpawnCB, m_mtCheckpoint, m_mtDeleteCB;
		sf::Text m_mtPointText, m_mtWallText, m_mtInstWallText, m_mtSpawnText, m_mtCheckpointText, m_mtDeleteText;

		// Grid size
		UI::UITextField m_gridSizeTF;
		UI::UICheckBox m_gridVisibleCB, m_gridLockCB;
		sf::Text m_gridSizeText, m_gridVisibleText, m_gridLockText;

		// Save & Menu
		UI::UIButton m_saveBtn, m_menuBtn;
		UI::UIPopup m_savePop;

		// Saves the map to a given file
		void SaveMap(std::string filename);

		// Recalculates the grid for the editor
		void RecalculateGrid();

		// Returns the point on the grid where the mouse is snapped to
		sf::Vector2f GetMousePointOnGrid();

		// Events
		void OnMousePressed(sf::Mouse::Button btn);
		void OnMouseReleased(sf::Mouse::Button btn);
		void OnMouseMoved(sf::Vector2i mousePos);
		void OnKeyPressed(sf::Keyboard::Key key);
		void OnWindowResized(sf::Vector2u size);

		// Changes the mouse type
		void ChangeMouseType(MouseType type);

	public:

		// Gets the map editor being used in the program
		static MapEditor& GetMapEditor();

		// Resets the map editor
		void Reset();

		// Initialises the map editor
		void Init();
		// Updates the map editor (includes drawing to window)
		void Update();
		// Loads the map editor
		void Load();
		// Unloads the map editor
		void Unload();

	};
}
