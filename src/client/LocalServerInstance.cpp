//
// Created by aurailus on 2019-10-31.
//

#include "LocalServerInstance.h"
#include "util/Log.h"
#include "server/Server.h"
#include <iostream>

LocalServerInstance::LocalServerInstance(const std::string &path, unsigned short port, const std::string& subgame) :
    port(port),
    path(path),
    subgame(subgame) {}

bool LocalServerInstance::start() {
#ifndef _WIN32
    pid = fork();
    if (pid == 0) {
        char *arr[] = {
            const_cast <char*>(""),
//            const_cast <char*>("-iconic"),
            const_cast <char*>("-e"),
            const_cast <char*>(path.data()),
            const_cast <char*>("--mode=server"),
            const_cast <char*>("--subgame=zeus"), //TODO: Don't hardcode this
            static_cast<char*>(nullptr)
        };

        execvp("xterm", arr);
    }
    return true;
#else
    std::thread([&]() {
       Server s(port, subgame);
    }).detach();
    return true;
#endif
}

void LocalServerInstance::stop() {
    #ifndef _WIN32
        if (pid != 0) kill(pid, SIGKILL);
    #else
        std::cout << Log::err << "Local Server destructor not implemented on Windows!" << Log::endl;
    #endif
}

LocalServerInstance::~LocalServerInstance() {
    stop();
}