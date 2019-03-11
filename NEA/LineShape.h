#pragma once

#include "test.h"


class LineShape : public sf::Drawable
{
private:
	sf::Vector2f m_points[2];
	sf::Vertex m_vertices[4];
	sf::Color m_colour;
	float m_thickness = 1.f;

	void CalculateLine(float thickness);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:

	LineShape();
	LineShape(sf::Vector2f p1, sf::Vector2f p2);

	void SetPoint(sf::Vector2f point, size_t index);
	void SetLineThickness(float thickness);
	void SetLineColour(sf::Color colour);
};