//
// Created by aurailus on 2020-08-06.
//

#pragma once

#include <shared_mutex>

using MutexType = std::shared_timed_mutex;
using ReadLock  = std::shared_lock<MutexType>;
using WriteLock = std::unique_lock<MutexType>;

class Lockable {
public:
    ReadLock getReadLock() const {
        return ReadLock {const_cast<MutexType&>(mutex)};
    };

    WriteLock getWriteLock() {
        return WriteLock {mutex};
    };

private:
    MutexType mutex;
};