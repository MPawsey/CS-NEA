#pragma once

#include <string>
#include "TextField.h"

namespace Functions
{
	std::string DoubleToString(double d);
	void ClampTextFieldDoubleValue(UI::TextField& text, double boundLower, double boundUpper);
	void ClampTextFieldIntegerValue(UI::TextField& text, int boundLower, int boundUpper);
}