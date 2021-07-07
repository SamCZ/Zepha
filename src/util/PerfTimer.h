#pragma once

#include "Timer.h"

class PerfTimer {
public:
	PerfTimer(const vec<string>& sections);
	
	void start(const string& section);
	vec<usize> end();
private:
	void stopCurrent();
	
	vec<string> sections;
	vec<usize> timings = {};
	string currentSection;
	Timer timer = Timer();
};


