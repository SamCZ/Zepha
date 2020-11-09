//
// Created by aurailus on 02/12/18.
//

#pragma once

#include <string>
#include <chrono>

class Timer {
	public:
	explicit Timer();
	
	explicit Timer(const std::string& name);
	
	long elapsedNs() const;
	
	void printElapsedNs();
	
	void printElapsedMs();
	
	void printElapsedSeconds();
	
	private:
	std::string name = "";
	std::chrono::high_resolution_clock::time_point start;
};

