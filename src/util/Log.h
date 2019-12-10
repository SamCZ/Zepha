//
// Created by aurailus on 12/06/19.
//

#pragma once

#include <string>
#include <iostream>

namespace Log {
    #ifdef _WIN32
    static const char* info = "[INFO] ";
    static const char* err  = "[ERR!] ";
    static const char* endl = "\n";

    static void clear() { auto s = system("cls"); }
    #else
    static const char* info = "\033[36m[INFO] ";
    static const char* err  = "\033[31m[ERR!] ";
    static const char* endl = "\033[0m\n";

    static void clear() { auto s = system("clear"); }
    #endif
}