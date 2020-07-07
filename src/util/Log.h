//
// Created by aurailus on 12/06/19.
//

#pragma once

namespace Log {
    #ifdef _WIN32
    static const char* info = "[info] ";
    static const char* warn = "[warn] ";
    static const char* err  = "[err] ";
    static const char* endl = "\n";

    static const char* undl = "";
    static const char* bold = "";
    static const char* unbl = "";
    static const char* red  = "";

    static void clear() { auto s = system("cls"); }
    #else
    static const char* info = "\033[36m[info] ";
    static const char* warn = "\033[31m[warn] ";
    static const char* err  = "\033[31m[err] ";
    static const char* endl = "\033[0m\n";

    static const char* undl = "\033[4m";
    static const char* bold = "\033[1m";
    static const char* unbl = "\033[4m\033[1m";
    static const char* red  = "\033[31m";

    static void clear() { auto s = system("clear"); }
    #endif
}