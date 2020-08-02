//
// Created by aurailus on 2020-08-02.
//

#pragma once

#include <memory>

template <typename B, typename L, typename S,
    typename = typename std::enable_if<std::is_base_of<B, S>::value>::type,
    typename = typename std::enable_if<std::is_base_of<B, L>::value>::type>
class CovariantPtr {
    std::shared_ptr<B> b;
public:
    CovariantPtr(std::shared_ptr<B> ptr) : b(ptr) {}
    CovariantPtr(std::shared_ptr<L> ptr) : b(std::dynamic_pointer_cast<B>(ptr)) {}
    CovariantPtr(std::shared_ptr<S> ptr) : b(std::dynamic_pointer_cast<B>(ptr)) {}

    std::shared_ptr<B> operator->() { return b; }
    std::shared_ptr<L> l() { auto ptr = std::dynamic_pointer_cast<L>(b); if (!ptr) throw std::bad_cast(); return ptr; }
    std::shared_ptr<S> s() { auto ptr = std::dynamic_pointer_cast<S>(b); if (!ptr) throw std::bad_cast(); return ptr; }
};