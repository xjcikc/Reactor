#ifndef __THREAD_H__
#define __THREAD_H__

#include "Noncopyable.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>

using std::function;

class Thread
: public Noncopyable {
    using ThreadCallback = function<void()>;
public:
    Thread(ThreadCallback &&cb);
    virtual ~Thread();

    void start();
    void stop();
private:
    static void* threadFunc(void*);
private:
    pthread_t _thid;
    bool _isRunning;
    ThreadCallback _cb;
};

#endif