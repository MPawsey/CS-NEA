#include "MapEditor.h"
#include "Window.h"
#include "InputManager.h"
#include "Functions.h"
#include "MenuManager.h"

#define PI 3.141592653

namespace Editor
{
	


	MapEditor& MapEditor::GetMapEditor()
	{
		static MapEditor mapEditor;
		return mapEditor;
	}

	int MapEditor::FindLastPotentialConnection(unsigned int point)
	{
		// Binary Search
		// If pos1 is found, the pointer is moved left until it finds pos1 that isn't 

		// If no connections exist, a first connection cannot exist
		if (m_connections.size() == 0)
		{
			return -1;
		}

		// Initialise the id to the centre and find the respective pos1
		unsigned int size = m_connections.size() / 2;
		unsigned int id = size; 
		unsigned int lastId = m_connections.size() + 1;
		unsigned int pos = (*std::next(m_connections.begin(), id)).pos1;

		// Keep looping until the pos is the one we are looking for
		while (pos != point)
		{
			// A solution does not exist
			if ((size = (size + 1) / 2) == 0)
			{
				if (pos < point)
					return id;
				return -1;
			}

			// Changes id to new value
			if (pos > point)
			{
				id -= size;
			}
			else
			{
				id += size;
			}

			lastId = id;

			// Finds the new pos
			pos = (*std::next(m_connections.begin(), id)).pos1;
		}

		// Keeps cycling left until we get to the start of the chain
		while (pos == point)
		{
			id += 1;
			// Just in case we are at the first value in the list
			if (id == m_connections.size())
				break;
			pos = (*std::next(m_connections.begin(), id)).pos1;
		}

		// Returns the id of the first element in the list
		return id - 1;
	}

	
	void MapEditor::OnKeyPressed(sf::Keyboard::Key key)
	{
		if (!m_isActive)
			return;

		// Sets the mouse type depending on what key was pressed
		bool reset = false;
		switch (key)
		{
		case sf::Keyboard::P:
			ChangeMouseType(Point);
			reset = true;
			break;
		case sf::Keyboard::W:
			ChangeMouseType(Wall);
			break;
		case sf::Keyboard::I:
			ChangeMouseType(InstantWall);
			break;
		case sf::Keyboard::S:
			ChangeMouseType(Spawn);
			reset = true;
			break;
		case sf::Keyboard::C:
			ChangeMouseType(Checkpoint);
			reset = true;
			break;
		case sf::Keyboard::D:
			ChangeMouseType(Delete);
			reset = true;
			break;
		case sf::Keyboard::G:
			m_gridVisible = !m_gridVisible;
			m_gridVisibleCB.SetChecked(m_gridVisible);
			break;
		case sf::Keyboard::L:
			m_gridLock = !m_gridLock;
			m_gridLockCB.SetChecked(m_gridLock);
			break;
		case sf::Keyboard::Add:
			m_gridSize += 25.f;
			m_gridSizeTF.SetRawText(Functions::DoubleToString((double)m_gridSize));
			RecalculateGrid();
			break;
		case sf::Keyboard::Subtract:
			if ((m_gridSize -= 25.f) < 25.f)
				m_gridSize = 25.f;
			m_gridSizeTF.SetRawText(Functions::DoubleToString((double)m_gridSize));
			RecalculateGrid();
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
				sf::Vector2f pos = m_ghost.getPosition();

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
								
								// Checks to see if this connection exists
								for (auto c : m_connections)
								{
									if (c.pos1 == pos1 && c.pos2 == pos2)
									{
										// Connection exists so no need to create it
										m_points[m_selectedPoint].setFillColor(sf::Color::White);
										m_selectedPoint = i;
										m_points[m_selectedPoint].setFillColor(sf::Color::Green);
										return;
									}
								}

								m_connections.push_back(Connection{ sf::Vertex{ m_points[m_selectedPoint].getPosition(), sf::Color::White}, sf::Vertex{ m_points[i].getPosition(), sf::Color::White }, pos1, pos2 });
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
				sf::Vector2f pos = m_ghost.getPosition();

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

					// Checks to see if this connection exists
					for (auto c : m_connections)
					{
						if (c.pos1 == pos1 && c.pos2 == pos2)
						{
							// Connection exists so no need to create it
							m_points[m_selectedPoint].setFillColor(sf::Color::White);
							m_selectedPoint = index;
							m_points[m_selectedPoint].setFillColor(sf::Color::Green);
							return;
						}
					}

					m_connections.push_back(Connection{ sf::Vertex{ m_points[m_selectedPoint].getPosition(), sf::Color::White}, sf::Vertex{ m_points[index].getPosition(), sf::Color::White }, pos1, pos2 });
					m_points[m_selectedPoint].setFillColor(sf::Color::White);
					m_selectedPoint = index;
				}
				break;
			}
			case Spawn:
			{
				// Gets the position of the point
				sf::Vector2f pos = m_ghost.getPosition();


				m_spawnPos.setPosition(pos);
				m_spawnRotPressed = true;
				break;
			}
			case Checkpoint:
			{
				m_isDrawingCheckpoint = true;
				sf::CircleShape cp;
				cp.setFillColor(sf::Color::Transparent);
				cp.setOutlineColor(sf::Color::Cyan);
				cp.setPosition(m_ghost.getPosition());
				cp.setOutlineThickness(1.f);

				sf::CircleShape p{ 5.f };
				p.setFillColor(sf::Color::Cyan);
				p.setOrigin(5.f, 5.f);
				p.setPosition(m_ghost.getPosition());

				sf::Text t{std::to_string(m_checkPoints.size() + 1), UI::GetFont()};
				t.setPosition(m_ghost.getPosition() + sf::Vector2f{5.f, -2.5f});
				t.setFillColor(sf::Color::Cyan);
				t.setCharacterSize(12);

				m_checkPoints.push_back({cp, p, t});
				break;
			}
			case Delete:
			{
				// Gets the position of the point
				sf::Vector2f pos = InputManager::GetMousePosInView(m_editorView);

				// Check to see if a point around this position exists
				for (unsigned int i = 0; i < m_points.size(); ++i)
				{
					if (Functions::Distance(pos, m_points[i].getPosition()) <= m_points[i].getRadius())
					{
						// The position lies within a point, so the point can be deleted

						for (int j = 0; j < m_connections.size(); ++j)
						{
							if (m_connections[j].pos1 == i || m_connections[j].pos2 == i)
							{
								m_connections.erase(m_connections.begin() + j);
								--j;
								continue;
							}
							
							if (m_connections[j].pos1 > i)
							{
								m_connections[j].pos1--;
							}

							if (m_connections[j].pos2 > i)
							{
								m_connections[j].pos2--;
							}

						}

						m_points.erase(m_points.begin() + i);
						break;
					}
				}

				// Check to see if the user clicked a checkpoint centre
				for (unsigned int i = 0; i < m_checkPoints.size(); ++i)
				{
					if (Functions::Distance(pos, m_checkPoints[i].m_centre.getPosition()) <= m_checkPoints[i].m_centre.getRadius())
					{
						// The position lies within a point, so the point can be deleted
						m_checkPoints.erase(m_checkPoints.begin() + i);

						for (unsigned int j = i; j < m_checkPoints.size(); ++j)
						{
							m_checkPoints[j].m_idText.setString(std::to_string(j + 1));
						}

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

		if (btn == sf::Mouse::Left)
		{
			m_spawnRotPressed = false;
			m_isDrawingCheckpoint = false;
		}
		else if (btn == sf::Mouse::Middle)
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
		m_gridLines.clear();

		// Ensures the grid is always centred correctly
		m_gridView.setCenter(std::fmodf(m_editorView.getCenter().x, m_gridSize), std::fmodf(m_editorView.getCenter().y, m_gridSize));

		// Calculates bounds of grid
		float left, right, top, bottom;
		left = -(m_gridView.getSize().x / 2.f) - m_gridSize;
		right = (m_gridView.getSize().x / 2.f) + m_gridSize;
		top = -(m_gridView.getSize().y / 2.f) - m_gridSize;
		bottom =  (m_gridView.getSize().y / 2.f) + m_gridSize;

		// Finds the left and top most line positions
		float xStart = (std::floorf(left / m_gridSize) * m_gridSize);
		float yStart = (std::floorf(top / m_gridSize) * m_gridSize);

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


	void MapEditor::ChangeMouseType(MouseType type)
	{
		// No need to do anything if already checked
		if (m_mouseType == type)
			return;

		m_mouseType = type;

		// Resets the checkboxes
		m_mtPointCB.SetChecked(false);
		m_mtWallCB.SetChecked(false);
		m_mtInstWallCB.SetChecked(false);
		m_mtSpawnCB.SetChecked(false);
		m_mtCheckpoint.SetChecked(false);
		m_mtDeleteCB.SetChecked(false);

		// Checks the corresponding checkbox
		switch (type)
		{
		case Point:
			m_mtPointCB.SetChecked(true);
			break;
		case Wall:
			m_mtWallCB.SetChecked(true);
			break;
		case InstantWall:
			m_mtInstWallCB.SetChecked(true);
			break;
		case Spawn:
			m_mtSpawnCB.SetChecked(true);
			break;
		case Checkpoint:
			m_mtCheckpoint.SetChecked(true);
			break;
		case Delete:
			m_mtDeleteCB.SetChecked(true);
			break;
		}

		// Unselects the current selected point
		if (m_selectedPoint != -1)
		{
			m_points[m_selectedPoint].setFillColor(sf::Color::White);
			m_selectedPoint = -1;
		}
	}

	// Saves the map with the name given in text
	void MapEditor::SaveMap(std::string filename)
	{
		std::string filepath = "Tracks/" + filename + ".track";
		std::ofstream file = std::ofstream{ filepath };

		// Saves the points on the track
		for (auto p : m_points)
		{
			Functions::WriteLineToFile(file, 'p', p.getPosition().x, p.getPosition().y);
		}

		// Saves the walls (connections)
		for (auto c : m_connections)
		{
			Functions::WriteLineToFile(file, 'w', c.pos1, c.pos2);
		}

		// Saves the spawn
		Functions::WriteLineToFile(file, 's', m_spawnPos.getPosition().x, m_spawnPos.getPosition().y, m_spawnPos.getRotation());

		// Saves the checkpoints
		for (auto cp : m_checkPoints)
		{
			Functions::WriteLineToFile(file, 'c', cp.m_centre.getPosition().x, cp.m_centre.getPosition().y, cp.m_outline.getRadius());
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

		m_UIView.setViewport(sf::FloatRect{ 0.f, 0.9f, 1.f, 0.1f });
		m_UIView.setSize(window.getSize().x * 1.5f, window.getSize().y * 0.15f);
		m_UIView.setCenter(m_UIView.getSize() / 2.f);

		// Setup the initial grid
		RecalculateGrid();

		// Setup the spawn car
		m_spawnPos.setFillColor(sf::Color::Transparent);
		m_spawnPos.setOutlineColor(sf::Color::White);
		m_spawnPos.setOutlineThickness(1.f);
		m_spawnPos.setSize(sf::Vector2f{ 15.f, 25.f });
		m_spawnPos.setOrigin(m_spawnPos.getSize() / 2.f);
		m_spawnPos.setRotation(180);
		
		m_spawnDir = LineShape{ m_spawnPos.getPosition(), m_spawnPos.getPosition() + sf::Vector2f{0.f, 12.5f} };
		m_spawnDir.SetLineColour(sf::Color::Red);
		m_spawnDir.SetLineThickness(2.f);

		// Setup the ghost point
		m_ghost.setFillColor(sf::Color{ 255, 128, 128, 128 });
		m_ghost.setRadius(5.f);
		m_ghost.setOrigin(5.f, 5.f);

		// Setup the UI

		// Mouse types
		m_mtPointCB = UI::CheckBox{ m_UIView };
		m_mtPointCB.setPosition(5.f, 5.f);
		m_mtPointCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(Point); else ChangeMouseType(None); });
		m_mtPointCB.SetTooltipText("Point");
		m_mtPointCB.SetChecked(true);
		m_mtPointText = sf::Text{ "P", UI::GetFont() };
		m_mtPointText.setPosition(5.f, 35.f);

		m_mtWallCB = UI::CheckBox{ m_UIView };
		m_mtWallCB.setPosition(50.f, 5.f);
		m_mtWallCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(Wall); else ChangeMouseType(None); });
		m_mtWallCB.SetTooltipText("Wall");
		m_mtWallText = sf::Text{ "W", UI::GetFont() };
		m_mtWallText.setPosition(50.f, 35.f);

		m_mtInstWallCB = UI::CheckBox{ m_UIView };
		m_mtInstWallCB.setPosition(100.f, 5.f);
		m_mtInstWallCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(InstantWall); else ChangeMouseType(None); });
		m_mtInstWallCB.SetTooltipText("Instant Wall");
		m_mtInstWallText = sf::Text{ "I", UI::GetFont() };
		m_mtInstWallText.setPosition(100.f, 35.f);

		m_mtCheckpoint = UI::CheckBox{ m_UIView };
		m_mtCheckpoint.setPosition(150.f, 5.f);
		m_mtCheckpoint.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(Checkpoint); else ChangeMouseType(None); });
		m_mtCheckpoint.SetTooltipText("Checkpoint");
		m_mtCheckpointText = sf::Text{ "C", UI::GetFont() };
		m_mtCheckpointText.setPosition(150.f, 35.f);

		m_mtSpawnCB = UI::CheckBox{ m_UIView };
		m_mtSpawnCB.setPosition(200.f, 5.f);
		m_mtSpawnCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(Spawn); else ChangeMouseType(None); });
		m_mtSpawnCB.SetTooltipText("Spawn");
		m_mtSpawnText = sf::Text{ "S", UI::GetFont() };
		m_mtSpawnText.setPosition(200.f, 35.f);

		m_mtDeleteCB = UI::CheckBox{ m_UIView };
		m_mtDeleteCB.setPosition(250.f, 5.f);
		m_mtDeleteCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { if (checked) ChangeMouseType(Delete); else ChangeMouseType(None); });
		m_mtDeleteCB.SetTooltipText("Delete");
		m_mtDeleteText = sf::Text{ "D", UI::GetFont() };
		m_mtDeleteText.setPosition(250.f, 35.f);

		// Grid settings
		// Grid size
		m_gridSizeTF = UI::TextField{ 200.f, UI::TextField::Decimal, m_UIView, {2.5f, 5.f, 0.f, 0.f} };
		m_gridSizeTF.setPosition(350.f, 5.f);
		m_gridSizeTF.SetRawText("50.0");
		m_gridSizeTF.SetTooltipText("[>0.0] Default = 50.0\nSets the grid size");
		m_gridSizeTF.GetLostFocusEvent().AddCallback([&]() 
		{ 
			if (m_gridSizeTF.GetFloatValue() == 0.f) 
				m_gridSizeTF.SetRawText("1.0"); 
			m_gridSize = m_gridSizeTF.GetFloatValue();
			RecalculateGrid();
		});
		m_gridSizeText = sf::Text{ "Grid Size", UI::GetFont() };
		m_gridSizeText.setPosition(350.f, 35.f);

		// Grid visible
		m_gridVisibleCB = UI::CheckBox{ m_UIView };
		m_gridVisibleCB.setPosition(600.f, 5.f);
		m_gridVisibleCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { m_gridVisible = checked; });
		m_gridVisibleCB.SetTooltipText("Grid Visible");
		m_gridVisibleCB.SetChecked(true);
		m_gridVisibleText = sf::Text{ "G", UI::GetFont() };
		m_gridVisibleText.setPosition(600.f, 35.f);

		// Grid lock
		m_gridLockCB = UI::CheckBox{ m_UIView };
		m_gridLockCB.setPosition(650.f, 5.f);
		m_gridLockCB.GetCheckBoxUpdateEvent().AddCallback([&](bool checked) { m_gridLock = checked; });
		m_gridLockCB.SetTooltipText("Grid Lock");
		m_gridLockCB.SetChecked(true);
		m_gridLockText = sf::Text{ "L", UI::GetFont() };
		m_gridLockText.setPosition(650.f, 35.f);


		// Save UI
		m_savePop = UI::Popup{ m_UIView };
		m_savePop.SetViewportTransform({ 0.25f, 0.25f, 0.5f, 0.5f });
		m_savePop.SetViewSize((sf::Vector2f)Window::GetWindowSize() * 0.5f);

		UI::TextField* nameTF = new UI::TextField{ 300, UI::TextField::FieldType::Text, m_savePop.GetView() };
		nameTF->SetTooltipText("The name of the save file.");
		nameTF->SetRawText("Track");
		nameTF->setPosition((m_savePop.GetView().getSize().x - nameTF->GetBounds().width) / 2.f, 50.f);
		m_savePop.AddElement(nameTF); // No deleting of retBtn as m_savePopup destructor deletes it

		UI::Button* retBtn = new UI::Button{ "Cancel", m_savePop.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		retBtn->setPosition(5.f, m_savePop.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		retBtn->GetMouseClickedEvent().AddCallback([&]() { m_savePop.SetActive(false); Load(); });
		retBtn->SetCentreText(true);
		m_savePop.AddElement(retBtn);

		UI::Button* saveBtn = new UI::Button{ "Save", m_savePop.GetView(), UI::Padding{2.5f, 5.f, 0.f, 0.f} };
		saveBtn->setPosition(m_savePop.GetView().getSize().x - retBtn->GetBounds().width - 5.f, m_savePop.GetView().getSize().y - retBtn->GetBounds().height - 5.f);
		saveBtn->GetMouseClickedEvent().AddCallback([&, nameTF]() { SaveMap(nameTF->GetRawText()); m_savePop.SetActive(false); Load(); });
		saveBtn->SetCentreText(true);
		m_savePop.AddElement(saveBtn);

		m_saveBtn = UI::Button{ "Save Map", m_UIView, {2.5f, 5.f, 0.f, 0.f} };
		m_saveBtn.SetCentreText(true);
		m_saveBtn.setPosition((window.getSize().x * 1.5f) - m_saveBtn.GetClickBounds().width - 5.f, 5.f);
		m_saveBtn.GetMouseClickedEvent().AddCallback([&]() { Unload(); m_savePop.SetActive(true); });

		// Menu
		m_menuBtn = UI::Button{ "Menu", m_UIView, {2.5f, 5.f, 0.f, 0.f} };
		m_menuBtn.SetBackgroundSize(sf::Vector2f{ m_saveBtn.GetClickBounds().width, m_saveBtn.GetClickBounds().height });
		m_menuBtn.SetCentreText(true);
		m_menuBtn.setPosition((window.getSize().x * 1.5f) - m_saveBtn.GetClickBounds().width - 5.f, 50.f);
		m_menuBtn.GetMouseClickedEvent().AddCallback([&]() { Unload(); Window::SetWindowState(Window::Menu); Menu::MenuManager::GetMenuManager().GoToState(Menu::MenuState::MainMenu); });
	}

	void MapEditor::Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		// Updates ghost point
		if (m_gridLock)
			m_ghost.setPosition(GetMousePointOnGrid());
		else
			m_ghost.setPosition(InputManager::GetMousePosInView(m_editorView));

		// Updates the spawn rotation if needed
		if (m_spawnRotPressed)
		{
			// Incase of division by 0
			if (m_spawnPos.getPosition().x == m_ghost.getPosition().x)
			{
				// Sets the angle to either 0 or 180
				if (m_spawnPos.getPosition().y > m_ghost.getPosition().y)
					m_spawnPos.setRotation(0);
				else
					m_spawnPos.setRotation(180);
			}
			else
			{
				// Sets the angle to point towards the ghost point
				float angle = (atanf((m_spawnPos.getPosition().y - m_ghost.getPosition().y) / (m_spawnPos.getPosition().x - m_ghost.getPosition().x)) * 180.f / PI) + 90.f;
				if (m_ghost.getPosition().x < m_spawnPos.getPosition().x)
					angle = 180.f + angle;
				m_spawnPos.setRotation(angle);
			}


			m_spawnDir = LineShape{ m_spawnPos.getPosition(), m_spawnPos.getPosition() + sf::Vector2f{sinf(m_spawnPos.getRotation() * PI / 180.f), -cosf(m_spawnPos.getRotation() * PI / 180.f)} * 12.5f };
			m_spawnDir.SetLineColour(sf::Color::Red);
			m_spawnDir.SetLineThickness(2.f);
		}

		// Updates the checkpoint if rotation
		if (m_isDrawingCheckpoint)
		{
			m_checkPoints.back().m_outline.setRadius(Functions::Distance(m_checkPoints.back().m_outline.getPosition(), m_ghost.getPosition()));
			m_checkPoints.back().m_outline.setOrigin(m_checkPoints.back().m_outline.getRadius(), m_checkPoints.back().m_outline.getRadius());
		}


		// Draw the grid first so everything is drawn above it
		if (m_gridVisible)
		{
			window.setView(m_gridView);
			for (auto l : m_gridLines)
			{
				window.draw(l.data(), 2, sf::Lines);
			}
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

		// Draws the car
		window.draw(m_spawnPos);
		window.draw(m_spawnDir);

		// Draws the checkpoints
		for (auto cp : m_checkPoints)
		{
			window.draw(cp.m_outline);
			window.draw(cp.m_centre);
			window.draw(cp.m_idText);
		}

		// Draws the ghost point if needed
		if (m_mouseType == MouseType::Point || m_mouseType == MouseType::InstantWall || m_mouseType == MouseType::Spawn || m_mouseType == MouseType::Checkpoint)
		{
			window.draw(m_ghost);
		}



		// UI
		window.setView(m_UIView);

		window.draw(m_mtCheckpoint);
		window.draw(m_mtDeleteCB);
		window.draw(m_mtInstWallCB);
		window.draw(m_mtPointCB);
		window.draw(m_mtSpawnCB);
		window.draw(m_mtWallCB);

		window.draw(m_mtCheckpointText);
		window.draw(m_mtDeleteText);
		window.draw(m_mtInstWallText);
		window.draw(m_mtPointText);
		window.draw(m_mtSpawnText);
		window.draw(m_mtWallText);

		window.draw(m_gridSizeTF);
		window.draw(m_gridVisibleCB);
		window.draw(m_gridLockCB);
		window.draw(m_gridSizeText);
		window.draw(m_gridVisibleText);
		window.draw(m_gridLockText);

		window.draw(m_saveBtn);
		window.draw(m_savePop);
		window.draw(m_menuBtn);
	}

	void MapEditor::Load()
	{
		m_isActive = true;

		m_mtCheckpoint.SetActive(true);
		m_mtDeleteCB.SetActive(true);
		m_mtInstWallCB.SetActive(true);
		m_mtPointCB.SetActive(true);
		m_mtSpawnCB.SetActive(true);
		m_mtWallCB.SetActive(true);

		m_gridSizeTF.SetActive(true);
		m_gridVisibleCB.SetActive(true);
		m_gridLockCB.SetActive(true);

		m_saveBtn.SetActive(true);
		m_menuBtn.SetActive(true);
	}

	void MapEditor::Unload()
	{
		m_isActive = false;

		m_mtCheckpoint.SetActive(false);
		m_mtDeleteCB.SetActive(false);
		m_mtInstWallCB.SetActive(false);
		m_mtPointCB.SetActive(false);
		m_mtSpawnCB.SetActive(false);
		m_mtWallCB.SetActive(false);

		m_gridSizeTF.SetActive(false);
		m_gridVisibleCB.SetActive(false);
		m_gridLockCB.SetActive(false);

		m_saveBtn.SetActive(false);
		m_menuBtn.SetActive(false);
	}






}