//
// Created by aurailus on 02/12/18.
//

#include "Timer.h"

Timer::Timer(const char* name) {
     this->name = name;
     start = std::chrono::high_resolution_clock::now();
}

void Timer::elapsed() {
     auto finish = std::chrono::high_resolution_clock::now();

     long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();

     printf("%s took %ld ns.\n", this->name, elapsed);
}

void Timer::elapsedInMs() {
     auto finish = std::chrono::high_resolution_clock::now();

     double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() / (double)1000000;

     printf("%s took %.2f ms.\n", this->name, elapsed);
}

