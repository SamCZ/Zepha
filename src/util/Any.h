//
// Created by aurailus on 2020-03-23.
//

#pragma once

#include <typeinfo>

class Any {
public:
    Any() = default;

    template <typename T> static Any from(const T& val) {
        Any a;
        a.set<T>(val);
        return a;
    }

    template <typename T> void set(const T& val) noexcept {
        data = std::make_shared<T>(std::move(val));
        type = typeid(T).hash_code();
    }

    template<typename T> const T& get() const {
        if (empty()) throw std::logic_error("Tried to get empty Any.");
        else if (type != typeid(T).hash_code()) throw std::logic_error("Any is not of type specified.");
        return *std::static_pointer_cast<T>(data);
    }

    template<typename T> const T& get_or(const T& other) const noexcept {
        try { return get<T>(); }
        catch (...) { return other; }
    }

    template <typename T> const bool is() const noexcept {
        return typeid(T).hash_code() == type;
    }

    bool const empty() const noexcept {
        return !data;
    }

    void reset() noexcept {
        data = nullptr;
        type = 0;
    }

private:
    std::size_t type = 0;
    std::shared_ptr<void> data = nullptr;
};