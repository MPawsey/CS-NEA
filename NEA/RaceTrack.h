#pragma once

#include <string>
#include <sfml/Graphics.hpp>

namespace RaceTrack
{
	// Loads a track from a file
	void LoadFromFile(std::string filename);

	// Updates the track (include drawing to window)
	void Update();

	// Returns the number of checkpoints in the current track
	const unsigned int GetCheckpointCount();
	// Returns the start position for the current track
	const sf::Vector2f GetStartPos();
	// Returns the start rotation for the current track
	const float GetStartRot();
	// Returns the track distance for the track
	const float GetTrackDistance();
	// Returns the name of the current track
	const std::string GetTrackName();

	// Checks whether a line segment collides with any walls
	// The position of the collision is returned through out
	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f& out);
	// Checks whether a quad has collided with any walls
	bool CheckCollisions(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);
	// Checks whether a position vector lies inside a checkpoint
	bool CheckCheckpoints(sf::Vector2f pos, unsigned int nextCheckpoint);

	// Returns the distance from a given position and the checkpoint it is attempting to get to
	float CalcFitness(sf::Vector2f pos, unsigned int nextCheckpoint);

	// Sets whether the checkpoints should be visible
	void SetCheckpointsVisible(bool isVisible);

}