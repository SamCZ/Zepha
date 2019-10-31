//
// Created by aurailus on 2019-10-31.
//

#pragma once

#include <string>

#ifndef _WIN32
#include <zconf.h>
#include <signal.h>
#endif

class LocalServerInstance {
public:
    LocalServerInstance(const std::string& path, unsigned short port);

    bool start();
    void stop();

    unsigned short port = 0;
private:
    void initServer();

    std::string path;
    int pid = 0;
};
