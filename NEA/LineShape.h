#pragma once

#include <sfml/Graphics.hpp>


class LineShape : public sf::Drawable
{
private:
	sf::Vector2f m_points[2];
	sf::Vertex m_vertices[4];
	sf::Color m_colour;
	float m_thickness = 1.f;

	// Calculates the line
	void CalculateLine(float thickness);

	// Draws the line to the target
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:

	// Constructors
	
	// Creates and initialises a blank line shape (default constructor)
	LineShape();
	// Creates and initialises a line shape
	LineShape(sf::Vector2f p1, sf::Vector2f p2);

	// Sets one of the points of the line
	void SetPoint(sf::Vector2f point, size_t index);
	// Sets the thickness of the line
	void SetLineThickness(float thickness);
	// Sets the colour of the line
	void SetLineColour(sf::Color colour);
};