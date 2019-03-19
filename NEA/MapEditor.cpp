#include "MapEditor.h"
#include "Window.h"
#include "InputManager.h"
#include "Functions.h"

namespace Editor
{
	


	MapEditor& MapEditor::GetMapEditor()
	{
		static MapEditor mapEditor;
		return mapEditor;
	}

	
	void MapEditor::OnKeyPressed(sf::Keyboard::Key key)
	{
		// Sets the mouse type depending on what key was pressed
		bool reset = false;
		switch (key)
		{
		case sf::Keyboard::P:
			m_mouseType = Point;
			reset = true;
			break;
		case sf::Keyboard::W:
			m_mouseType = Wall;
			break;
		case sf::Keyboard::I:
			m_mouseType = InstantWall;
			break;
		case sf::Keyboard::S:
			m_mouseType = Spawn;
			reset = true;
			break;
		case sf::Keyboard::C:
			m_mouseType = Checkpoint;
			reset = true;
			break;
		case sf::Keyboard::D:
			m_mouseType = Delete;
			reset = true;
			break;
		}

		if (reset && m_selectedPoint != -1)
		{
			m_points[m_selectedPoint].setFillColor(sf::Color::White);
			m_selectedPoint = -1;
		}
	}

	void MapEditor::OnMousePressed(sf::Mouse::Button btn)
	{
		if (!m_isActive)
			return;

		if (btn == sf::Mouse::Middle && InputManager::IsMouseInView(m_editorView))
		{
			m_editorGrabbed = true;
			m_lastMousePos = sf::Mouse::getPosition(Window::GetWindow());
		}
		else if (btn == sf::Mouse::Right && InputManager::IsMouseInView(m_editorView))
		{
			if (m_selectedPoint != -1)
			{
				m_points[m_selectedPoint].setFillColor(sf::Color::White);
				m_selectedPoint = -1;
			}
		}
		else if (btn == sf::Mouse::Left && InputManager::IsMouseInView(m_editorView))
		{
			// Does process depending on mouse type
			switch (m_mouseType)
			{
			case Point:
			{
				// Gets the position of the point
				sf::Vector2f pos;
				if (m_gridLock)
					pos = GetMousePointOnGrid();
				else
					pos = InputManager::GetMousePosInView(m_editorView);

				// Check to see if a point around this position already exists
				for (auto p : m_points)
				{
					if (Functions::Distance(pos, p.getPosition()) <= p.getRadius())
					{
						// The point lies within another point, so there is no need to create it
						return;
					}
				}

				// Creates a new point at the position
				sf::CircleShape p{ 5.f };
				p.setFillColor(sf::Color::White);
				p.setOrigin(5.f, 5.f);
				p.setPosition(pos);

				m_points.push_back(p);
				break;
			}
			case Wall:
			{
				// Gets the position of the point
				sf::Vector2f pos = InputManager::GetMousePosInView(m_editorView);

				// Check to see if a point around this position exists
				for (unsigned int i = 0; i < m_points.size(); i++)
				{
					if (Functions::Distance(pos, m_points[i].getPosition()) <= m_points[i].getRadius())
					{
						// If there is already a point selected, creates a line
						if (m_selectedPoint != -1)
						{
							if (m_selectedPoint != i)
							{
								unsigned int pos1 = (m_selectedPoint < i ? m_selectedPoint : i), pos2 = i + m_selectedPoint - pos1;
								m_connections.insert(Connection{ sf::Vertex{ m_points[m_selectedPoint].getPosition(), sf::Color::White}, sf::Vertex{ m_points[i].getPosition(), sf::Color::White }, pos1, pos2 });
								m_points[m_selectedPoint].setFillColor(sf::Color::White);
								m_selectedPoint = i;
								m_points[m_selectedPoint].setFillColor(sf::Color::Green);
							}
						}
						else
						{
							m_selectedPoint = i;
							m_points[m_selectedPoint].setFillColor(sf::Color::Green);
						}
						return;
					}
				}
				if (m_selectedPoint != -1)
				{
					m_points[m_selectedPoint].setFillColor(sf::Color::White);
					m_selectedPoint = -1;
				}
				break;
			}
			case InstantWall:
			{
				// Gets the position of the point
				sf::Vector2f pos;
				if (m_gridLock)
					pos = GetMousePointOnGrid();
				else
					pos = InputManager::GetMousePosInView(m_editorView);

				// Check to see if a point around this position already exists
				unsigned int index = 0;
				bool found = false;
				for (; index < m_points.size(); index++)
				{
					if (Functions::Distance(pos, m_points[index].getPosition()) <= m_points[index].getRadius())
					{
						// The point lies within another point, so there is no need to create it
						found = true;
						m_points[index].setFillColor(sf::Color::Green);
						break;
					}
				}

				if (!found)
				{
					// Creates a new point at the position
					sf::CircleShape p{ 5.f };
					p.setFillColor(sf::Color::Green);
					p.setOrigin(5.f, 5.f);
					p.setPosition(pos);

					m_points.push_back(p);
					index = m_points.size() - 1;
				}

				if (m_selectedPoint == -1)
				{
					m_selectedPoint = index;
				}
				else
				{
					unsigned int pos1 = (m_selectedPoint < index ? m_selectedPoint : index), pos2 = index + m_selectedPoint - pos1;
					m_connections.insert(Connection{ sf::Vertex{ m_points[m_selectedPoint].getPosition(), sf::Color::White}, sf::Vertex{ m_points[index].getPosition(), sf::Color::White }, pos1, pos2 });
					m_points[m_selectedPoint].setFillColor(sf::Color::White);
					m_selectedPoint = index;
				}
				break;
			}
			case Delete:
			{
				// Gets the position of the point
				sf::Vector2f pos = InputManager::GetMousePosInView(m_editorView);

				// Check to see if a point around this position exists
				for (auto i = m_points.begin(); i != m_points.end(); ++i)
				{
					if (Functions::Distance(pos, (*i).getPosition()) <= (*i).getRadius())
					{
						// The position lies within a point, so the point can be deleted
						m_points.erase(i);
						break;
					}
				}
				break;
			}
			}
		}
	}

	void MapEditor::OnMouseReleased(sf::Mouse::Button btn)
	{
		if (!m_isActive)
			return;

		if (btn == sf::Mouse::Middle)
		{
			m_editorGrabbed = false;
		}
	}

	void MapEditor::OnMouseMoved(sf::Vector2i mousePos)
	{
		if (!m_isActive)
			return;

		if (m_editorGrabbed)
		{
			// Calculates the vector change of mouse position between the last event and now
			// Updates the position of both the grid and the editor (grid locked to grid size)
			sf::Vector2i deltaPos = mousePos - m_lastMousePos;
			m_gridView.setCenter(std::fmodf(m_gridView.getCenter().x - deltaPos.x, m_gridSize), std::fmodf(m_gridView.getCenter().y - deltaPos.y, m_gridSize));
			m_editorView.setCenter(m_editorView.getCenter().x - deltaPos.x, m_editorView.getCenter().y - deltaPos.y);
		}

		m_lastMousePos = mousePos;
	}

	sf::Vector2f MapEditor::GetMousePointOnGrid()
	{
		sf::Vector2f mousePos = InputManager::GetMousePosInView(m_editorView);
		mousePos.x = std::roundf(mousePos.x / m_gridSize) * m_gridSize;
		mousePos.y = std::roundf(mousePos.y / m_gridSize) * m_gridSize;
		return mousePos;
	}

	void MapEditor::RecalculateGrid()
	{
		// Calculates bounds of grid
		float left, right, top, bottom;
		left = m_gridView.getCenter().x - (m_gridView.getSize().x / 2.f) - m_gridSize;
		right = m_gridView.getCenter().x + (m_gridView.getSize().x / 2.f) + m_gridSize;
		top = m_gridView.getCenter().y - (m_gridView.getSize().y / 2.f) - m_gridSize;
		bottom = m_gridView.getCenter().y + (m_gridView.getSize().y / 2.f) + m_gridSize;

		// Finds the left and top most line positions
		float xStart = std::floorf(left / m_gridSize) * m_gridSize;
		float yStart = std::floorf(top / m_gridSize) * m_gridSize;

		// Creates two vertices
		sf::Vertex vert1, vert2;
		vert1.color = vert2.color = sf::Color{ 64, 64, 64 };

		// Sets the vertex y positions
		vert1.position.y = top;
		vert2.position.y = bottom;

		// Creates the vertical lines and stores them in m_gridLines
		for (float x = xStart; x < right; x += m_gridSize)
		{
			vert1.position.x = x;
			vert2.position.x = x;
			m_gridLines.push_back({vert1, vert2});
		}

		// Sets the vertex x positions
		vert1.position.x = left;
		vert2.position.x = right;

		// Creates the vertical lines and stores them in m_gridLines
		for (float y = yStart; y < bottom; y += m_gridSize)
		{
			vert1.position.y = y;
			vert2.position.y = y;
			m_gridLines.push_back({ vert1, vert2 });
		}

	}

	void MapEditor::Init()
	{
		sf::RenderWindow& window = Window::GetWindow();

		// Setup Input Events
		InputManager::GetGenericMousePressedEvent().AddCallback(&MapEditor::OnMousePressed, *this);
		InputManager::GetGenericMouseReleasedEvent().AddCallback(&MapEditor::OnMouseReleased, *this);
		InputManager::GetMouseMovedEvent().AddCallback(&MapEditor::OnMouseMoved, *this);
		InputManager::GetGenericKeyPressedEvent().AddCallback(&MapEditor::OnKeyPressed, *this);
		
		// Setup the views
		m_gridView.setViewport(sf::FloatRect{ 0.f, 0.f, 1.f, 0.9f });
		m_gridView.setSize(window.getSize().x * 1.f, window.getSize().y * 0.9f);
		m_gridView.setCenter(0.f, 0.f);

		m_editorView = m_gridView;

		m_UIView.setViewport(sf::FloatRect{ 0.f, 0.8f, 1.f, 0.1f });
		m_UIView.setSize(window.getSize().x * 1.f, window.getSize().y * 0.1f);
		m_UIView.setCenter(m_UIView.getSize() / 2.f);

		// Setup the initial grid
		RecalculateGrid();

		// Setup the spawn car
		m_spawnPos.setFillColor(sf::Color::Transparent);
		m_spawnPos.setOutlineColor(sf::Color::White);
		m_spawnPos.setOutlineThickness(1.f);
		m_spawnPos.setSize(sf::Vector2f{ 15.f, 25.f });
		m_spawnPos.setRotation(180);
	}

	void MapEditor::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		// Draw the grid first so everything is drawn above it
		window.setView(m_gridView);
		for (auto l : m_gridLines)
		{
			window.draw(l.data(), 2, sf::Lines);
		}

		// Set view for the editor
		window.setView(m_editorView);

		// Draws the walls
		for (auto w : m_connections)
		{
			window.draw(w.m_line.data(), 2, sf::Lines);
		}

		// Draws the points
		for (auto p : m_points)
		{
			window.draw(p);
		}
	}

	void MapEditor::Load()
	{
		m_isActive = true;
	}

	void MapEditor::Unload()
	{
		m_isActive = false;
	}






}