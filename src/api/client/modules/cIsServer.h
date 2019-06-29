//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_CISSERVER_H
#define ZEUS_CISSERVER_H

#include <sol.hpp>

namespace ClientApi {
    void is_server(sol::table& zeus) {
        zeus.set_function("is_server", []() {
            return false;
        });
    }
}

#endif //ZEUS_CISSERVER_H
