//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_TIMER_H
#define GLPROJECT_TIMER_H

#include <iostream>
#include <chrono>

class Timer {
public:
    explicit Timer(const char* name);

    void elapsedNs();
    void elapsedMs();
    void elapsed(); //Seconds
private:
    const char* name;
    std::chrono::high_resolution_clock::time_point start;
};


#endif //GLPROJECT_TIMER_H
