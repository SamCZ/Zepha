#include <bits/stdc++.h>

#include "PerfTimer.h"

PerfTimer::PerfTimer(const vec<string>& sections) : sections(sections), timings(sections.size()) {}

void PerfTimer::start(const string& section) {
	stopCurrent();
	const usize ind = find(sections.begin(), sections.end(), section) - sections.begin();
	if (ind == sections.size()) throw std::invalid_argument("Tried to start invalid section '" + section + "'");
	timings[ind] = 0;
	timer = Timer();
	currentSection = section;
}

void PerfTimer::resume(const string& section) {
	stopCurrent();
	const usize ind = find(sections.begin(), sections.end(), section) - sections.begin();
	if (ind == sections.size()) throw std::invalid_argument("Tried to resume invalid section '" + section + "'");
	timer = Timer();
	currentSection = section;
}

vec<usize> PerfTimer::end() {
	stopCurrent();
	const auto copy = timings;
	for (usize i = 0; i < timings.size(); i++) timings[i] = 0;
	return copy;
}

void PerfTimer::stopCurrent() {
	if (currentSection != "") {
		const usize ind = find(sections.begin(), sections.end(), currentSection) - sections.begin();
		timings[ind] += timer.elapsedNs();
		currentSection = "";
	}
}


