#include "LineShape.h"

// Initialises a blank line shape
LineShape::LineShape() {}

// Initialises a line shape between two points
LineShape::LineShape(sf::Vector2f p1, sf::Vector2f p2)
{
	m_points[0] = p1;
	m_points[1] = p2;

	// Calculates the line
	CalculateLine(m_thickness);
}

// Calculates the line from a given thickness
void LineShape::CalculateLine(float thickness)
{
	// Determines the parallel and perpendicular vectors of the line
	sf::Vector2f direction = m_points[1] - m_points[0];
	sf::Vector2f unitPerpendicular = sf::Vector2f{ -direction.y, direction.x } / sqrtf(powf(direction.x, 2) + powf(direction.y, 2));

	// Calculates the offset of the line
	sf::Vector2f offset = unitPerpendicular * thickness / 2.f;

	// Positions each vertex for the line
	m_vertices[0].position = m_points[0] + offset;
	m_vertices[1].position = m_points[0] - offset;
	m_vertices[2].position = m_points[1] + offset;
	m_vertices[3].position = m_points[1] - offset;
}

// Draws the line to a target
void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices, 4, sf::Quads);
}

// Sets a point of the line
void LineShape::SetPoint(sf::Vector2f point, size_t index)
{
	// Only valid if index is 0 or 1
	if (index > 2)
		return;

	// Sets the point and recalculates the line
	m_points[index] = point;
	CalculateLine(m_thickness);
}

// Sets the thickness of the line
void LineShape::SetLineThickness(float thickness)
{
	// Updates the thickness of the line and recalculates the line
	CalculateLine(m_thickness = thickness);
}

// Sets the colour of the line
void LineShape::SetLineColour(sf::Color colour)
{
	// Updates the colour of the line
	m_colour = colour;

	// Updates the colour of each vertex
	for (sf::Vertex& v : m_vertices)
		v.color = m_colour;
}