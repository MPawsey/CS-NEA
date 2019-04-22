#pragma once

#include <string>
#include "TextField.h"
#include <fstream>

namespace Functions
{
	// Converts a double to a string
	std::string DoubleToString(double d);

	// Clamps a textfield between two double/integer values
	void ClampTextFieldDoubleValue(UI::UITextField& text, double boundLower, double boundUpper);
	void ClampTextFieldIntegerValue(UI::UITextField& text, int boundLower, int boundUpper);

	// Calculates the distance between two vectors
	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	// Fold expression to write a line to a file
	template <typename... Args>
	void WriteLineToFile(std::ofstream& file, Args&&... args)
	{
		// Folds over the first comma seperated expression resulting in a trailing space (should not matter)
		((file << args << " "), ...) << std::endl;
	}
}