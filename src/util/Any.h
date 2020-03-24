//
// Created by aurailus on 2020-03-23.
//

#pragma once

#include <memory>
#include <cstring>
#include <utility>
#include <typeinfo>
#include <functional>

class Any {
    constexpr static std::size_t STACK_SIZE = 32;
public:
    template<typename T, typename T_ = T> static Any from(T_&& val) {
        Any a {};
        a.set<T>(std::forward<T_>(val));
        return a;
    }

    template <typename T, typename T_ = T> void set(T_&& val) {
        type = typeid(T).hash_code();

        if (sizeof(T) <= STACK_SIZE) {
            new(data.s) T(std::forward<T_>(val));
            stack_destructor = std::bind(&Any::delete_stack<T_>, this);
        }
        else {
            data.h = std::make_shared<T>(std::forward<T_>(val));
            stack_destructor = nullptr;
        }
    }

    template<typename T> const T& get() const {
        if (empty()) throw std::logic_error("Tried to get empty Any.");
        if (type != typeid(T).hash_code()) throw std::logic_error("Any is not of type specified.");

        if (sizeof(T) <= STACK_SIZE) return *reinterpret_cast<T*>(const_cast<char*>(data.s));
        else return *std::static_pointer_cast<T>(data.h);
    }

    template<typename T> const T& get_or(const T& other) const noexcept {
        try { return get<T>(); }
        catch (...) { return other; }
    }

    template <typename T> const bool is() const noexcept {
        return typeid(T).hash_code() == type;
    }

    bool const empty() const noexcept {
        return type == 0;
    }

    void reset() noexcept {
        data.h = nullptr;
        stack_destructor = nullptr;
        type = 0;
    }

    ~Any() {
        if (stack_destructor) stack_destructor();
    }

private:
    template <typename T> void delete_stack() {
        reinterpret_cast<T*>(data.s)->~T();
    }

    std::size_t type = 0;
    std::function<void()> stack_destructor = nullptr;

    union Data {
        char s[STACK_SIZE];
        std::shared_ptr<void> h;

        Data() { this->h = nullptr; }
        Data(const Data& o) { memcpy(this->s, o.s, STACK_SIZE); }
        ~Data() {};
    } data {};
};