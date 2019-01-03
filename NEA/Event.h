#pragma once

#include <algorithm>
#include <functional>
#include <vector>

template <class... T>
class Event
{
private:
	std::vector<std::function<void(T...)>> callbacks;

public:

	template <typename C>
	void AddCallback(void(C::* function)(T...), C& c)
	{
		callbacks.push_back(std::bind(function, std::ref(c)));
	}

	void AddCallback(const std::function<void(T...)>& f)
	{
		callbacks.push_back(f);
	}

	void Call(T... t)
	{
		std::for_each(callbacks.begin(), callbacks.end(), [t...](std::function<void(T...)> f) { f(t...); });
	}

};