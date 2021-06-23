#pragma once

#include <chrono>

#include "util/Types.h"

/**
 * Tracks time at nanosecond precision, and provides methods to print
 * and retrieve the elapsed time since initialization.
 */

class Timer {
public:
	explicit Timer();
	
	explicit Timer(const std::string& name);
	
	u64 elapsedNs() const;
	
	void printElapsedNs();
	
	void printElapsedMs();
	
	void printElapsedSeconds();
	
private:
	string name = "";
	std::chrono::high_resolution_clock::time_point start;
};

