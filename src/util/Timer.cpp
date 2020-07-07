//
// Created by aurailus on 02/12/18.
//

#include <iostream>

#include "Timer.h"

#include "Log.h"

Timer::Timer() :
    start(std::chrono::high_resolution_clock::now()) {}

Timer::Timer(const char* name) :
    name(name), hasName(true),
    start(std::chrono::high_resolution_clock::now()) {}

long Timer::elapsedNs() const {
    auto finish = std::chrono::high_resolution_clock::now();
    long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    return elapsed;
};

void Timer::printElapsedNs() {
    if (!name) {
        std::cout << Log::err << "Timer without name called print function." << Log::endl;
        return;
    }

    printf("%s took %ld ns.\n", this->name, elapsedNs());
}

void Timer::printElapsedMs() {
    if (!name) {
        std::cout << Log::err << "Timer without name called print function." << Log::endl;
        return;
    }

    printf("%s took %.2f ms.\n", this->name, elapsedNs() / 1000000.);
}

void Timer::printElapsedSeconds() {
    if (!name) {
        std::cout << Log::err << "Timer without name called print function." << Log::endl;
        return;
    }

    printf("%s took %.2f secs.\n", this->name, elapsedNs() / 1000000. / 1000.);
}