#pragma once

#include <random>

namespace RNG
{

	class Random : public std::mt19937
	{
	private:
		unsigned int calls = 0;

	public:

		unsigned int operator()()
		{
			++calls;
			return static_cast<std::mt19937*>(this)->operator()();
		}

		const unsigned int GetCalls()
		{
			return calls;
		}
		
	};



}