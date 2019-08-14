//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_SISSERVER_H
#define ZEUS_SISSERVER_H

#include <sol2/sol.hpp>

namespace ServerApi {
    void is_server(sol::table& zeus) {
        zeus.set_function("is_server", []() {
            return true;
        });
    }
}

#endif //ZEUS_CISSERVER_H
