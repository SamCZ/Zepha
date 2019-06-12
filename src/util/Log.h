//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_LOG_H
#define ZEUS_LOG_H

#include <string>
#include <iostream>

namespace Log {
    static const char* info = "\033[36m[INFO] ";
    static const char* err  = "\033[31m[ERR!] ";
    static const char* endl = "\033[0m\n";
}

#endif //ZEUS_LOG_H
