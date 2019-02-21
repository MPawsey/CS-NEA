#pragma once

#include <string>
#include <sfml/Graphics.hpp>

namespace RaceTrack
{

	void LoadFromFile(std::string filename);
	void Update();

	const unsigned int GetCheckpointCount();
	const sf::Vector2f GetStartPos();
	const float GetStartRot();

	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& out);
	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);
	bool CheckCheckpoints(sf::Vector2f pos, unsigned int nextCheckpoint);

	float CalcFitness(sf::Vector2f pos, unsigned int nextCheckpoint);

	std::string GetTrackName();



}