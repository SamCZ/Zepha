//
// Created by aurailus on 02/12/18.
//

#pragma once

#include <iostream>
#include <chrono>

class Timer {
public:
    explicit Timer();
    explicit Timer(const char* name);

    long elapsedNs() const;

    void printElapsedNs();
    void printElapsedMs();
    void printElapsedSeconds();
private:
    bool hasName = false;
    const char* name = nullptr;
    std::chrono::high_resolution_clock::time_point start;
};

