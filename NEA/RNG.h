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

		unsigned int operator()()
		{
			++m_calls;
			return static_cast<std::mt19937*>(this)->operator()();
		}

		const unsigned long long GetCalls()
		{
			return m_calls;
		}

		const unsigned int GetSeed()
		{
			return m_seed;
		}

		void SetSeed(unsigned int s)
		{
			m_calls = 0;
			m_seed = s;
			seed(s);
		}
	};



}