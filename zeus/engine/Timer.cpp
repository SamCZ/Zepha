//
// Created by aurailus on 02/12/18.
//

#include "Timer.h"

Timer::Timer(const char* name) {
     this->name = name;
     start = std::chrono::high_resolution_clock::now();
}

long Timer::elapsedNs() {
    auto finish = std::chrono::high_resolution_clock::now();
    long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
    return elapsed;
};


void Timer::printElapsedNs() {
     printf("%s took %ld ns.\n", this->name, elapsedNs());
}

void Timer::printElapsedMs() {
     printf("%s took %.2f ms.\n", this->name, elapsedNs() / (double)1000000);
}

void Timer::printElapsedSeconds() {
    printf("%s took %.2f secs.\n", this->name, elapsedNs() / (double)1000000 / (double)1000);
}