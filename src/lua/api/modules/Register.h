//
// Created by aurailus on 2020-07-24.
//

#pragma once

#include "Base.h"

namespace Api::Module {
    class Register : public Api::Module::Base {
    public:
        using Base::Base;
        void bind() override;

    protected:
        void createRegisterFn(const std::string& name, const std::string& table);
        void registerFn(const std::string& table, sol::environment env, const std::string& identifier, const sol::table& data);
    };
}