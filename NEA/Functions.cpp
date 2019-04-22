#include "Functions.h"

#include <algorithm>

namespace Functions
{
	// Converts a double to a string
	std::string DoubleToString(double d)
	{
		// Converts the double to a string
		std::string s = std::to_string(d);

		// Returns a string without trailing 0s
		return s.erase(s.find_last_not_of('0') + 1, std::string::npos).back() == '.' ? (s.pop_back(), s) : s;
	}

	// Clamps the text of a text field between an upper and lower double bound
	void ClampTextFieldDoubleValue(UI::UITextField& text, double boundLower, double boundUpper)
	{
		// Sets the text to 0 if no text exists
		if (text.GetRawText().getSize() == 0)
			text.SetRawText("0");
		// Sets the text of the field to a clamped value
		text.SetRawText(DoubleToString(std::clamp(std::stod(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	// Clamps the text of a text field between an upper and lower integer bound
	void ClampTextFieldIntegerValue(UI::UITextField& text, int boundLower, int boundUpper)
	{
		// Sets the text to 0 if no text exists
		if (text.GetRawText().getSize() == 0)
			text.SetRawText("0");
		// Sets the text of the field to a clamped value
		text.SetRawText(DoubleToString(std::clamp(std::stoi(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	// Calculates the distance between two vectors using pythagoras
	float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		return sqrtf(powf(p2.x - p1.x, 2) + powf(p2.y - p1.y, 2));
	}
}