#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <queue>
#include <functional>

using std::queue;
using std::function;

using ElemType = function<void()>;

class TaskQueue {
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();

    void push(ElemType &&task);
    ElemType pop();

    void wakeup();
    bool empty() const;
    bool full() const;
private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};

#endif