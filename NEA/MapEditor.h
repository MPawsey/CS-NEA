#pragma once

#include <sfml/Graphics.hpp>
#include <vector>
#include <array>
#include "LineShape.h"
#include <set>

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

		bool m_isActive = false;
		MouseType m_mouseType = MouseType::Point;
		int m_selectedPoint = -1;

		sf::View m_gridView, m_editorView, m_UIView;

		std::vector<sf::CircleShape> m_points;
		std::set<Connection> m_connections;
		std::vector<sf::CircleShape> m_checkPoints;
		sf::RectangleShape m_spawnPos;
		bool m_editorGrabbed = false;
		sf::Vector2i m_lastMousePos;

		bool m_gridLock = true;
		float m_gridSize = 50.f;
		std::vector<std::array<sf::Vertex, 2>> m_gridLines;

		void RecalculateGrid();
		sf::Vector2f GetMousePointOnGrid();

		void OnMousePressed(sf::Mouse::Button btn);
		void OnMouseReleased(sf::Mouse::Button btn);
		void OnMouseMoved(sf::Vector2i mousePos);
		void OnKeyPressed(sf::Keyboard::Key key);

	public:


		static MapEditor& GetMapEditor();

		void Init();
		void Update();
		void Load();
		void Unload();

	};
}
