#include "LineShape.h"


LineShape::LineShape()
{

}

LineShape::LineShape(sf::Vector2f p1, sf::Vector2f p2)
{
	m_points[0] = p1;
	m_points[1] = p2;
}

void LineShape::CalculateLine(float thickness)
{
	sf::Vector2f direction = m_points[1] - m_points[0];
	sf::Vector2f unitPerpendicular = sf::Vector2f{ -direction.y, direction.x } / sqrtf(powf(direction.x, 2) + powf(direction.y, 2));

	sf::Vector2f offset = unitPerpendicular * thickness / 2.f;

	m_vertices[0].position = m_points[0] + offset;
	m_vertices[1].position = m_points[0] - offset;
	m_vertices[2].position = m_points[1] + offset;
	m_vertices[3].position = m_points[1] - offset;
}

void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices, 4, sf::Quads);
}

void LineShape::SetPoint(sf::Vector2f point, size_t index)
{
	if (index > 2)
		return;

	m_points[index] = point;
	CalculateLine(m_thickness);
}

void LineShape::SetLineThickness(float thickness)
{
	CalculateLine(m_thickness = thickness);
}

void LineShape::SetLineColour(sf::Color colour)
{
	m_colour = colour;

	for (sf::Vertex& v : m_vertices)
		v.color = m_colour;
}