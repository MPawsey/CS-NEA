#pragma once

#include <string>
#include "TextField.h"
#include <fstream>

namespace Functions
{
	std::string DoubleToString(double d);
	void ClampTextFieldDoubleValue(UI::TextField& text, double boundLower, double boundUpper);
	void ClampTextFieldIntegerValue(UI::TextField& text, int boundLower, int boundUpper);
	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	// Fold expression to write a line to a file
	template <typename... Args>
	void WriteLineToFile(std::ofstream& file, Args&&... args)
	{
		// Folds over the first comma seperated expression resulting in a trailing space (should not matter)
		((file << args << " "), ...) << std::endl;
	}
}