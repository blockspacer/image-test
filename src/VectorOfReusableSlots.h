#pragma once

#include <vector>
#include <functional>

template <typename T>
class VectorOfReusableSlots {
	std::vector<bool> inUse;
	std::vector<T> items;
public:
	size_t getFreeSlot(T &item) {
		size_t i = 0;
		while (true) {
			if (i == items.size()) {
				items.push_back(item);
				inUse.push_back(true);
				return i;
			}
			if ( ! inUse[i]) {
				items[i] = item;
				inUse[i] = true;
				return i;
			}

			++i;
		}
	}
	void markUnused(size_t n) {
		inUse[n] = false;
	}
	T& operator[]       (size_t i)       { return items[i]; }
	const T& operator[] (size_t n) const { return items[n]; }
	size_t size() { return items.size(); }
	void forEach(std::function<void(T&)> func) {
		for (size_t i=0; i<items.size(); ++i)
			if (inUse[i])
				func(items[i]);
	}
};


