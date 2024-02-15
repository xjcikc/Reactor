#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <stdio.h>

class MutexLock
: public Noncopyable {
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void trylock();
    void unlock();
    pthread_mutex_t* getMutexPtr();
private:
    pthread_mutex_t _mutex;
};

class MutexLockGuard {
public:
    MutexLockGuard(MutexLock &mutex): _mutex(mutex) {
        _mutex.lock();
    }
    ~MutexLockGuard() {
        _mutex.unlock();
    }
private:
    MutexLock &_mutex;
};

#endif