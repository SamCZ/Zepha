//
// Created by aurailus on 12/06/19.
//

#pragma once

#include <string>
#include <iostream>

namespace Log {
    #ifdef _WIN32
    static const char* info = "[INFO] ";
    static const char* warn = "[WARN] ";
    static const char* err  = "[ERR!] ";
    static const char* endl = "\n";

    static const char* undl = "";
    static const char* bold = "";
    static const char* unbl = "";
    static const char* red  = "";

    static void clear() { auto s = system("cls"); }
    #else
    static const char* info = "\033[36m[INFO] ";
    static const char* warn = "\033[31m[WARN] ";
    static const char* err  = "\033[31m[ERR!] ";
    static const char* endl = "\033[0m\n";

    static const char* undl = "\033[4m";
    static const char* bold = "\033[1m";
    static const char* unbl = "\033[4m\033[1m";
    static const char* red  = "\033[31m";

    static void clear() { auto s = system("clear"); }
    #endif
}