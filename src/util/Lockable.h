//
// Created by aurailus on 2020-08-06.
//

#pragma once

#include <shared_mutex>

using mutex_type = std::shared_timed_mutex;
using read_lock  = std::shared_lock<mutex_type>;
using write_lock = std::unique_lock<mutex_type>;

class Lockable {
public:
    read_lock getReadLock() const {
        return read_lock {const_cast<mutex_type&>(mutex)};
    };

    write_lock getWriteLock() {
        return write_lock {mutex};
    };

private:
    mutex_type mutex;
};