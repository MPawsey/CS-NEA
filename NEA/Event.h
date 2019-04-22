#pragma once

#include <algorithm>
#include <functional>
#include <vector>

// A struct for events that allow events to be removed
struct EventID
{
	unsigned int m_ID = 0, startPos = 0;
	bool m_valid = false;
};

// Creates an event that has arguments equal to the datatypes passed into the template
template <class... T>
class Event
{
private:
	std::vector<std::function<void(T...)>> m_callbacks;
	std::vector<unsigned int> m_removedPositions;
	
	bool m_inUse = false;
	std::vector<size_t> m_removedPos;

public:
	
	// Constructors

	// Creates a blank event (Default constructor)
	Event() = default;

	// Creates and initialises an event from another event (copy constructor)
	Event(const Event<T...>& evnt)
	{
		m_callbacks = std::vector<std::function<void(T...)>>{ evnt.m_callbacks};
		m_removedPositions = std::vector<unsigned int>{ evnt.m_removedPositions };
	}

	// Sets the values of the current event from another event
	Event& operator=(const Event<T...>& evnt)
	{
		m_callbacks = std::vector<std::function<void(T...)>>{ evnt.m_callbacks };
		m_removedPositions = std::vector<unsigned int>{ evnt.m_removedPositions };

		// Returns reference to this so calls can be chained
		return *this;
	}

	// Adds a function to the list of callbacks
	// This should be used when the function comes from a class
	// Returns the ID of the event
	template <class C>
	EventID AddCallback(void(C::* function)(T...), C& c)
	{
		m_callbacks.push_back([&, function](T... t) { std::invoke(function, c, t...); });
		return { m_callbacks.size() - 1, m_removedPositions.size(), true };
	}

	// Adds a function ot the list of callbacks
	// This should be used when the function is static
	// Returns the ID of the event
	EventID AddCallback(const std::function<void(T...)>& f)
	{
		m_callbacks.push_back(f);
		return { m_callbacks.size() - 1, m_removedPositions.size(), true };
	}

	// Removes a callback from the callback list
	// Takes an ID and removes the callback from the event
	void RemoveCallback(EventID id)
	{
		// Checks to see if the id is valid
		if (!id.m_valid) return;

		// Finds the real position of the id (after callbacks have been removed)
		int pos = id.m_ID;
		if (m_removedPositions.size() > 0)
			std::for_each(m_removedPositions.begin() + id.startPos, m_removedPositions.end(), [&](unsigned int i) { if (pos > i) --pos; });

		// Checks if the position is valid
		if (pos < m_callbacks.size())
		{
			// If the call is currently happening, appends the position of the callback from the start of the call
			// such that the iterator can be decremented if needed (so that it doesn't miss any callbacks)
			if (m_inUse)
			{
				// Finds the correct position after the callbacks have been removed
				size_t newPos = pos;
				for (auto i : m_removedPos)
				{
					if (i < newPos)
					{
						newPos += 1;
					}
				}
				// Adds the new position to the removed list
				m_removedPos.push_back(newPos);
			}
			
			// Erases the callback
			m_callbacks.erase(m_callbacks.begin() + pos);
			m_removedPositions.push_back(pos);
		}
	}

	// Calls the event
	// Requires arguments that matches the template
	void Call(T... t)
	{
		// Sets the event so that it is currently being used
		m_inUse = true;

		// Iterates through all of the functions in the callback list
		// Using this method so if a callback is removed during the calling of callbacks an error doesn't arise
		for (auto it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
		{
			// Clears the removed pos list
			m_removedPos.clear();

			// Calls the function
			(*it)(t...);

			// Fixes the iterator if any callbacks were removed
			for (auto i : m_removedPos)
			{
				if (i > it - m_callbacks.begin())
				{
					--it;
				}
			}
		}
		// Unlocks the event
		m_inUse = false;
	}
};