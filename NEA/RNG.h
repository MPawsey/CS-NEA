#pragma once

#include <random>

namespace RNG
{

	class Random : public std::mt19937
	{
	private:
		unsigned long long m_calls = 0;
		unsigned int m_seed;

	public:

		// Process called when the variable is used like a function
		unsigned int operator()()
		{
			// Increments m_calls and returns the operator() for this as an STL random class
			++m_calls;
			return static_cast<std::mt19937*>(this)->operator()();
		}

		// Returns the number of times the RNG was called
		const unsigned long long GetCalls() const
		{
			return m_calls;
		}

		// Returns the seed of the RNG
		const unsigned int GetSeed() const
		{
			return m_seed;
		}

		// Sets the seed of the RNG
		void SetSeed(unsigned int s)
		{
			// Sets the values to their initial state
			m_calls = 0;
			m_seed = s;
			seed(s);
		}
	};



}