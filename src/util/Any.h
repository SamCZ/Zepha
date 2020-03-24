//
// Created by aurailus on 2020-03-23.
//

#pragma once

#include <typeinfo>

class Any {
public:
    Any() = default;
    template <typename T> Any(T* val) {
        set<T>(val);
    }

    template <typename T> void set(T* val) {
        type = typeid(T).hash_code();
        hasData = true;
        data = val;
    }

    template <typename T> T* get() {
        if (!hasData || type != typeid(T).hash_code()) return nullptr;
        return static_cast<T*>(data);
    }

    bool empty() {
        return !hasData;
    }

    template <typename T> void erase() {
        if (!hasData) return;
        hasData = false;
        delete static_cast<T*>(data);
        type = 0;
    }

private:
    std::size_t type = 0;
    bool hasData = false;
    void* data = nullptr;
};