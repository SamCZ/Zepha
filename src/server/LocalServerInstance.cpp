//
// Created by aurailus on 2019-10-31.
//

#include "LocalServerInstance.h"
#include "../util/Log.h"
#include <iostream>

LocalServerInstance::LocalServerInstance(const std::string &path, unsigned short port) :
    port(port),
    path(path) {}

bool LocalServerInstance::start() {
#ifdef _WIN32
    std::cout << Log::err << "Local Server not implemented on Windows! [1]" << Log::endl;
    return false;
#else
    pid = fork();
    if (pid == 0) initServer();
    return true;
#endif
}

void LocalServerInstance::initServer() {
    //This method is called on the Server Process.
    //TODO: Implement version for Win32.

    char *arr[] = {
            (char*) "",
//            (char*) "-iconic",
            (char*) "-e",
            const_cast<char *>(path.data()),
            (char*) "--mode=server",
            (char*) nullptr
    };

	#ifndef _WIN32
		execvp("xterm", arr);
	#endif
}

void LocalServerInstance::stop() {
#ifdef _WIN32
    std::cout << Log::err << "Local Server not implemented on Windows! [2]" << Log::endl;
#else
    if (pid != 0) kill(pid, SIGKILL);
#endif
}
