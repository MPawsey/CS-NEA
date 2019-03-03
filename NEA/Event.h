#pragma once

#include <algorithm>
#include <functional>
#include <vector>

struct EventID
{
	unsigned int m_ID = 0, m_startPos = 0;
	bool m_valid = false;
};

template <class... T>
class Event
{
private:
	std::vector<std::function<void(T...)>> m_callbacks;
	std::vector<unsigned int> m_removedPositions;
	

public:
	
	Event() = default;

	Event(const Event<T...>& e)
	{
		m_callbacks = std::vector<std::function<void(T...)>>{e.m_callbacks};
		m_removedPositions = std::vector<unsigned int>{ e.m_removedPositions };
	}

	Event& operator=(const Event<T...>& e)
	{
		m_callbacks = std::vector<std::function<void(T...)>>{ e.m_callbacks };
		m_removedPositions = std::vector<unsigned int>{ e.m_removedPositions };

		return *this;
	}

	// Returns the ID of the event
	template <typename C>
	EventID AddCallback(void(C::* function)(T...), C& c)
	{
		// I hate this piece of code. Had to copy the function pointer, not reference it as reference is destroyed.
		m_callbacks.push_back([&, function](T... t) { std::invoke(function, c, t...); });
		return { m_callbacks.size() - 1, m_removedPositions.size(), true };
	}

	// Returns the ID of the event
	EventID AddCallback(const std::function<void(T...)>& f)
	{
		m_callbacks.push_back(f);
		return { m_callbacks.size() - 1, m_removedPositions.size(), true };
	}

	// Takes an ID and removes the callback from the event
	void RemoveCallback(EventID id)
	{
		if (!id.m_valid) return;
		int pos = id.m_ID;
		if (m_removedPositions.size() > 0)
			std::for_each(m_removedPositions.begin() + id.m_startPos, m_removedPositions.end(), [&](unsigned int i) { if (pos > i) --pos; });
		m_removedPositions.push_back(pos);
		if (pos < m_callbacks.size()) // Just to stop errors
			m_callbacks.erase(m_callbacks.begin() + pos);
	}

	void Call(T... t)
	{
		// Using this method so if a callback is removed during the calling of callbacks an error doesn't arise
		for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
		{
			(*it)(t...);
		}
	}
};