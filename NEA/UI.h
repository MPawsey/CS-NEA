#pragma once

#include <sfml/Graphics/Font.hpp>
#include "Clickable.h"
#include "Button.h"
#include "TextField.h"


namespace UI
{
	struct Padding
	{
		float left = 0, right = 0, top = 0, bottom = 0;
	};

	void Init();

	const sf::Font& GetFont();

}