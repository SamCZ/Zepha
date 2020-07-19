//
// Created by aurailus on 02/12/18.
//

#include <iostream>

#include "Timer.h"

#include "Log.h"

Timer::Timer() :
    start(std::chrono::high_resolution_clock::now()) {}

Timer::Timer(const std::string& name) :
    name(name), start(std::chrono::high_resolution_clock::now()) {}

long Timer::elapsedNs() const {
    auto finish = std::chrono::high_resolution_clock::now();
    long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    return elapsed;
};

void Timer::printElapsedNs() {
    std::cout << this->name << " took " << elapsedNs() << " ns." << std::endl;;
}

void Timer::printElapsedMs() {
    std::cout << this->name << " took " << (elapsedNs() / 1000000.) << " ms." << std::endl;;
}

void Timer::printElapsedSeconds() {
    std::cout << this->name << " took " << (elapsedNs() / 1000000. / 1000.) << " secs." << std::endl;
}