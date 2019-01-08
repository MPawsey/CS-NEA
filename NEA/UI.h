#pragma once

#include <sfml/Graphics/Font.hpp>

namespace UI
{
	struct Padding
	{
		float left = 0, right = 0, top = 0, bottom = 0;
	};

	void Init();

	const sf::Font& GetFont();

}


// Ensures the header files are included after the above has been defined
#include "Clickable.h"
#include "Button.h"
#include "TextField.h"
#include "Slider.h"