#include "Functions.h"

#include <algorithm>

namespace Functions
{
	std::string DoubleToString(double d)
	{
		std::string s = std::to_string(d);
		return s.erase(s.find_last_not_of('0') + 1, std::string::npos).back() == '.' ? (s.pop_back(), s) : s;
	}

	void ClampTextFieldDoubleValue(UI::TextField& text, double boundLower, double boundUpper)
	{
		if (text.GetRawText().getSize() == 0)
			text.SetRawText("0");
		text.SetRawText(DoubleToString(std::clamp(std::stod(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}

	void ClampTextFieldIntegerValue(UI::TextField& text, int boundLower, int boundUpper)
	{
		if (text.GetRawText().getSize() == 0)
			text.SetRawText("0");
		text.SetRawText(DoubleToString(std::clamp(std::stoi(text.GetRawText().toAnsiString()), boundLower, boundUpper)));
	}
}