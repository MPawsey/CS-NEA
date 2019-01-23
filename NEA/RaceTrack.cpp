#pragma once

#include "RaceTrack.h"
#include "Window.h"
#include <vector>
#include <array>
#include <sstream>
#include <fstream>
#include <numeric>
#include "EvolutionManager.h"

namespace RaceTrack
{
	// Private
	struct Checkpoint
	{
		sf::Vector2f pos;
		float radius;
	};

	std::vector<std::array<sf::Vertex, 2>> m_walls;
	std::vector<Checkpoint> m_checkpoints;
	std::vector<float> m_checkpointDistances;

	sf::Vector2f m_startPos;
	float m_startRot;


	// Finds collision between two segments
	// out = point of collision
	// returns true if segments intersect
	bool Raycast(const sf::Vector2f& a1, const sf::Vector2f& a2, const sf::Vector2f& b1, const sf::Vector2f& b2, sf::Vector2f& out)
	{
		// Can they collide?
		float maxAx = std::max(a1.x, a2.x);
		float minAx = std::min(a1.x, a2.x);
		float maxBx = std::max(b1.x, b2.x);
		float minBx = std::min(b1.x, b2.x);
		float maxAy = std::max(a1.y, a2.y);
		float minAy = std::min(a1.y, a2.y);
		float maxBy = std::max(b1.y, b2.y);
		float minBy = std::min(b1.y, b2.y);

		if (maxAx < minBx || minAx > maxBx ||
			maxAy < minBy || minAy > maxBy)
			return false;

		// if X values are equal, cannot work out gradient (divide by 0)
		if (a1.x == a2.x)
		{
			if (b1.x == b2.x)
			{
				if (b1.x == a1.x)
				{
					out.x = b1.x;
					out.y = b1.y;
					return true;
				}
				return false;
			}

			float mb = (b2.y - b1.y) / (b2.x - b1.x);
			float cb = (b1.y - mb * b1.x);

			float y = mb * a1.x + cb;

			if (y >= minAy && y >= minBy && y <= maxAy && y <= maxBy)
			{
				out.x = a1.x;
				out.y = y;

				return true;
			}

			return false;
		}

		if (b1.x == b2.x)
		{
			float ma = (a2.y - a1.y) / (a2.x - a1.x);
			float ca = (a1.y - ma * a1.x);

			float y = ma * b1.x + ca;

			if (y >= minAy && y >= minBy && y <= maxAy && y <= maxBy)
			{
				out.x = b1.x;
				out.y = y;

				return true;
			}

			return false;
		}

		// get gradient
		float ma = (a2.y - a1.y) / (a2.x - a1.x);
		float mb = (b2.y - b1.y) / (b2.x - b1.x);


		// cannot collide if m is the same
		if (ma == mb)
			return false;

		// Find c
		float ca = (a1.y - ma * a1.x);
		float cb = (b1.y - mb * b1.x);

		float x = (cb - ca) / (ma - mb);

		// if x is outside, cannot intersect
		if (x > maxAx || x < minAx || x > maxBx || x < minBx)
		{
			return false;
		}

		// x is within both
		out.x = x;
		out.y = (ma * x) + ca;

		return true;
	}

	float Magnitude(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
	}

	// Public
	void LoadFromFile(std::string filename)
	{
		std::ifstream file{ filename };
		std::vector<sf::Vector2f> points;

		if (file.is_open())
		{
			// Parse file
			while (!file.eof())
			{
				std::string s;

				std::getline(file, s);
				std::stringstream ss;
				ss << s;

				char junk;
				float x, y;

				if (s[0] == 'p')
				{
					ss >> junk >> x >> y;
					points.emplace_back(x, y);
				}
				else if (s[0] == 'w')
				{
					int w1, w2;
					ss >> junk >> w1 >> w2;
					if (w1 >= points.size() || w2 >= points.size())
					{
						continue;
					}
					m_walls.push_back({ sf::Vertex{points[w1], sf::Color::White}, sf::Vertex{points[w2], sf::Color::White} });
				}
				else if (s[0] == 's')
				{
					ss >> junk >> x >> y >> m_startRot;
					m_startPos = { x, y };
					m_checkpoints.insert(m_checkpoints.begin(), { m_startPos, 1.f });
				}
				else if (s[0] == 'c')
				{
					float r;
					ss >> junk >> x >> y >> r;
					m_checkpoints.push_back({ sf::Vector2f{x, y}, r });
				}
			}

			// Accumulate checkpoints
			m_checkpointDistances.push_back(0.f);
			for (unsigned int i = 1; i < m_checkpoints.size(); i++)
				m_checkpointDistances.push_back(Magnitude(m_checkpoints[i].pos, m_checkpoints[i-1].pos));

			std::partial_sum(m_checkpointDistances.begin(), m_checkpointDistances.end(), m_checkpointDistances.begin());

			Evolution::EvolutionManager::ResetCars();
		}
	}

	void Update()
	{
		sf::RenderWindow& window = Window::GetWindow();

		for (auto wall : m_walls)
		{
			window.draw(wall.data(), 2, sf::LinesStrip);
		}

		sf::CircleShape s;
		s.setFillColor(sf::Color::Transparent);
		s.setOutlineColor(sf::Color::White);
		s.setOutlineThickness(1.f);
		for (auto cp : m_checkpoints)
		{
			s.setRadius(cp.radius);
			s.setOrigin(cp.radius, cp.radius);
			s.setPosition(cp.pos);
			window.draw(s);
		}
	}

	const unsigned int GetCheckpointCount()
	{
		return m_checkpoints.size();
	}

	const sf::Vector2f GetStartPos()
	{
		return m_startPos;
	}

	const float GetStartRot()
	{
		return m_startRot;
	}

	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& out)
	{
		bool collided = false;

		// From p1 to p2
		for (auto wall : m_walls)
		{
			for (int i = 1; i < wall.size(); i++)
			{
				if (Raycast(p1, p2, wall[i - 1].position, wall[i].position, out))
				{
					p2 = out;
					collided = true;
				}
			}
		}

		return collided;
	}

	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4)
	{
		sf::Vector2f out;

		for (auto wall : m_walls)
		{
			for (unsigned int i = 1; i < wall.size(); i++)
			{
				if (Raycast(p1, p2, wall[i - 1].position, wall[i].position, out) ||
					Raycast(p2, p3, wall[i - 1].position, wall[i].position, out) ||
					Raycast(p3, p4, wall[i - 1].position, wall[i].position, out) ||
					Raycast(p4, p1, wall[i - 1].position, wall[i].position, out))
					return true;
			}
		}

		return false;
	}

	bool CheckCheckpoints(sf::Vector2f pos, unsigned int nextCheckpoint)
	{
		if (nextCheckpoint == m_checkpoints.size())
		{
			return false;
		}
		return Magnitude(pos, m_checkpoints[nextCheckpoint].pos) <= m_checkpoints[nextCheckpoint].radius;
	}

	float CalcFitness(sf::Vector2f pos, unsigned int nextCheckpoint)
	{
		if (nextCheckpoint == m_checkpoints.size())
		{
			return m_checkpointDistances.back() + Magnitude(pos, m_checkpoints.back().pos);
		}
		return m_checkpointDistances[nextCheckpoint] - Magnitude(pos, m_checkpoints[nextCheckpoint].pos);
	}
}