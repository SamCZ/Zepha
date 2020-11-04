//
// Created by aurailus on 2019-10-31.
//

#pragma once

#include <string>

#ifndef _WIN32
#include <thread>
#include <zconf.h>
#include <signal.h>
#endif

class LocalServerInstance {
public:
    LocalServerInstance(const std::string& path, unsigned short port, const std::string& subgame);

    bool start();
    void stop();

    ~LocalServerInstance();
private:
    std::string path;
    std::string subgame;
    unsigned short port = 0;

    int pid = 0;
};
