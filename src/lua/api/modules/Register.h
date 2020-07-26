//
// Created by aurailus on 2020-07-24.
//

#pragma once

#include "Module.h"

namespace Api::Module {
    class Register : public Api::Module::Module {
    public:
        using Module::Module;
        void bind() override;

    protected:
        void createRegisterFn(const std::string& name, const std::string& table = "");
        void registerFn(const std::string& table, sol::environment env, const std::string& identifier, const sol::table& data);
    };
}