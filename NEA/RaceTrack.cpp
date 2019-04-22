#pragma once

#include "RaceTrack.h"
#include "Window.h"
#include <vector>
#include <array>
#include <sstream>
#include <fstream>
#include <numeric>
#include "EvolutionManager.h"
#include "Functions.h"
#include <algorithm>

namespace RaceTrack
{
	// Struct for the checkpoints
	struct Checkpoint
	{
		sf::Vector2f pos;
		float radius;
	};

	// Track variables
	std::vector<std::array<sf::Vertex, 2>> walls;
	std::string trackName;
	sf::Vector2f startPos;
	float startRot;

	// Checkpoint variables
	std::vector<Checkpoint> checkpoints;
	std::vector<float> checkpointDistances;
	std::vector<sf::CircleShape> checkpointCircles;
	bool showCheckpoints = true;

	// Finds collision between two segments
	// out = point of collision
	// returns true if segments intersect
	bool DoLinesIntersect(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2, sf::Vector2f& out)
	{
		// Gets the min and max x and y coords from the two line segments
		float maxAx = std::max(a1.x, a2.x);
		float minAx = std::min(a1.x, a2.x);
		float maxBx = std::max(b1.x, b2.x);
		float minBx = std::min(b1.x, b2.x);
		float maxAy = std::max(a1.y, a2.y);
		float minAy = std::min(a1.y, a2.y);
		float maxBy = std::max(b1.y, b2.y);
		float minBy = std::min(b1.y, b2.y);

		// Stops running if they cannot intersect (no overlap)
		if (maxAx < minBx || minAx > maxBx ||
			maxAy < minBy || minAy > maxBy)
			return false;

		// If X values are equal, cannot work out gradient (divide by 0)
		if (a1.x == a2.x)
		{
			// If the other two lines are parallel, the only way they can intersect is if they are along the same plane
			if (b1.x == b2.x)
			{
				if (b1.x == a1.x)
				{
					// They are along the same plane, so collision occurs
					out.x = b1.x;
					out.y = b1.y;
					return true;
				}
				return false;
			}

			// Calculates the gradient and constant of b
			float mb = (b2.y - b1.y) / (b2.x - b1.x);
			float cb = (b1.y - mb * b1.x);

			// Calculates the y value at the x value of the vertical line
			float y = mb * a1.x + cb;

			// Checks to see if the line intersects
			if (y >= minAy && y >= minBy && y <= maxAy && y <= maxBy)
			{
				out.x = a1.x;
				out.y = y;

				return true;
			}

			return false;
		}

		// If X values are equal, cannot work out gradient (divide by 0)
		if (b1.x == b2.x)
		{
			// Calculates the gradient and constant of a
			float ma = (a2.y - a1.y) / (a2.x - a1.x);
			float ca = (a1.y - ma * a1.x);

			// Calculates the y value at the x value of the vertical line
			float y = ma * b1.x + ca;

			// Checks to see if the line intersects
			if (y >= minAy && y >= minBy && y <= maxAy && y <= maxBy)
			{
				out.x = b1.x;
				out.y = y;

				return true;
			}

			return false;
		}

		// Gets gradient for both lines
		float ma = (a2.y - a1.y) / (a2.x - a1.x);
		float mb = (b2.y - b1.y) / (b2.x - b1.x);


		// Cannot intersect if m is the same
		if (ma == mb)
			return false;

		// Find c
		float ca = (a1.y - ma * a1.x);
		float cb = (b1.y - mb * b1.x);

		// Finds the x value of the intersection
		float x = (cb - ca) / (ma - mb);

		// If x is outside, cannot intersect
		if (x > maxAx || x < minAx || x > maxBx || x < minBx)
		{
			return false;
		}

		// x is within both so an intersection must occur
		out.x = x;
		out.y = (ma * x) + ca;

		return true;
	}

	// Loads a track from a file
	void LoadFromFile(std::string filename)
	{
		// Clears the current track
		walls.clear();
		checkpoints.clear();
		checkpointDistances.clear();
		checkpointCircles.clear();

		// Opens the track file
		trackName = filename;
		std::replace(trackName.begin(), trackName.end(), ' ', '_');
		trackName = trackName.substr(trackName.find_first_of('\\')+1, trackName.find_last_of('.') - (trackName.find_first_of('\\') + 1));
		std::ifstream file{ filename };

		// Vector to store the points of the track
		std::vector<sf::Vector2f> points;

		// Checks to see if the file is open
		if (file.is_open())
		{
			// Creates a template checkpoint circle
			sf::CircleShape cpCircle;
			cpCircle.setFillColor(sf::Color::Transparent);
			cpCircle.setOutlineColor(sf::Color::Yellow);
			cpCircle.setOutlineThickness(1.f);

			// Reads the file until it reaches the end
			while (!file.eof())
			{
				// Reads the current line of the file
				std::string s;
				std::getline(file, s);
				std::stringstream ss;
				ss << s;

				// Temp variable to store the character at the beginning of the line
				char junk;

				// To store any x, y positions
				float x, y;

				// Reads the first character and process the line accordingly
				if (s[0] == 'p') // Point
				{
					ss >> junk >> x >> y;
					points.emplace_back(x, y);
				}
				else if (s[0] == 'w') // Wall
				{
					// Gets the two walls points
					int w1, w2;
					ss >> junk >> w1 >> w2;
					// Makes sure the wall is valid
					if (w1 >= points.size() || w2 >= points.size())
					{
						continue;
					}
					// Adds the wall to the list of walls
					walls.push_back({ sf::Vertex{points[w1], sf::Color::White}, sf::Vertex{points[w2], sf::Color::White} });
				}
				else if (s[0] == 's') // Start
				{
					ss >> junk >> x >> y >> startRot;
					startPos = { x, y };
					// Adds the starting checkpoint
					checkpoints.insert(checkpoints.begin(), { startPos, 1.f });
				}
				else if (s[0] == 'c') // Checkpoint
				{
					float r;
					ss >> junk >> x >> y >> r;
					// Adds the checkpoint
					checkpoints.push_back({ sf::Vector2f{x, y}, r });

					// Updates the template and adds it to the checkpoint circles list
					cpCircle.setRadius(r);
					cpCircle.setOrigin(r, r);
					cpCircle.setPosition(x, y);
					checkpointCircles.push_back(cpCircle);
				}
			}

			// Accumulate checkpoints
			checkpointDistances.push_back(0.f);
			for (unsigned int i = 1; i < checkpoints.size(); i++)
				checkpointDistances.push_back(Functions::Distance(checkpoints[i].pos, checkpoints[i-1].pos));

			// Calculates the distances from the start at each checkpoint
			std::partial_sum(checkpointDistances.begin(), checkpointDistances.end(), checkpointDistances.begin());

			// Sets the final checkpoint colour to green
			checkpointCircles.back().setOutlineColor(sf::Color::Green);

			// Resets the cars
			Evolution::EvolutionManager::GetEvolutionManager().ResetCars();
		}
	}

	// Updates the track (includes drawing to the window)
	void Update()
	{
		// Gets the window instance for the program
		sf::RenderWindow& window = Window::GetWindow();

		// Draws all of the walls to the window
		for (auto wall : walls)
		{
			window.draw(wall.data(), 2, sf::LinesStrip);
		}

		// Checks to see if checkpoints should be drawn
		if (showCheckpoints)
		{
			// Draws all of the checkpoints to the window
			for (auto cp : checkpointCircles)
			{
				window.draw(cp);
			}
		}
		else
		{
			// Draws the finish checkpoint if checkpoints aren't visible
			window.draw(checkpointCircles.back());
		}
	}

	// Returns the number of checkpoints in the current track
	const unsigned int GetCheckpointCount()
	{
		return checkpoints.size();
	}

	// Returns the starting position for the current track
	const sf::Vector2f GetStartPos()
	{
		return startPos;
	}

	// Returns the starting rotation (in degrees) for the current track
	const float GetStartRot()
	{
		return startRot;
	}

	// Returns the total distance for the track
	const float GetTrackDistance()
	{
		return checkpointDistances.back();
	}

	// Returns the name of the track
	const std::string GetTrackName()
	{
		return trackName;
	}

	// Checks whether a collision occured between a line segment and a wall
	// out = the point of collision
	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& out)
	{
		// Creates a collided flag as false
		bool collided = false;

		// Iterates through all of the walls in the track
		for (auto wall : walls)
		{
			// Checks to see if an intersection occured
			if (DoLinesIntersect(p1, p2, wall[0].position, wall[1].position, out))
			{
				// Update p2 to out and set collided to true
				p2 = out;
				collided = true;

				// Continue checking in case there is a closer wall
			}
		}

		// Returns whether a collision was detected
		return collided;
	}

	// Checks whether a collision occured between a quad of points and a wall
	// Quad works points act such that each point is connected to the one in front and behind
	// with p1 connecting to p4
	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4)
	{
		// Variable to catch output of DoLinesIntersect
		sf::Vector2f out;

		// Iterates through all of the walls in the track
		for (auto wall : walls)
		{
			// Returns true if either of the 4 line segments intersect with the wall
			if (DoLinesIntersect(p1, p2, wall[0].position, wall[1].position, out) ||
				DoLinesIntersect(p2, p3, wall[0].position, wall[1].position, out) ||
				DoLinesIntersect(p3, p4, wall[0].position, wall[1].position, out) ||
				DoLinesIntersect(p4, p1, wall[0].position, wall[1].position, out))
				return true;
		}

		// No collision found
		return false;
	}

	// Checks to see if a position lies inside of a checkpoint
	bool CheckCheckpoints(sf::Vector2f pos, unsigned int nextCheckpoint)
	{
		// Returns false if the checkpoint is invalid
		if (nextCheckpoint >= checkpoints.size())
		{
			return false;
		}
		// Returns whether the distance from the position to the checkpoint is less than the radius of the checkpoint
		return Functions::Distance(pos, checkpoints[nextCheckpoint].pos) <= checkpoints[nextCheckpoint].radius;
	}

	// Calculates the distance traversed through the track
	float CalcFitness(sf::Vector2f pos, unsigned int nextCheckpoint)
	{
		// Returns the length of the map if all checkpoints have been reached
		if (nextCheckpoint >= checkpoints.size())
		{
			return checkpointDistances.back();
		}
		// Returns the distance from the start to the next checkpoint minus the distance between the point and the next checkpoint
		return checkpointDistances[nextCheckpoint] - Functions::Distance(pos, checkpoints[nextCheckpoint].pos);
	}

	// Sets whether the checkpoints should be visible or not
	void SetCheckpointsVisible(bool isVisible)
	{
		showCheckpoints = isVisible;
	}
}