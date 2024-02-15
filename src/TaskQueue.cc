#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t queSize)
: _queSize(queSize)
, _notEmpty(_mutex)
, _notFull(_mutex)
, _flag(true) {}

TaskQueue::~TaskQueue() {}

void TaskQueue::push(ElemType &&task) {
    MutexLockGuard autoLock(_mutex);

    while(full()) {
        _notFull.wait();
    }
    _que.push(std::move(task));
    _notEmpty.notify();
}

ElemType TaskQueue::pop() {
    MutexLockGuard autoLock(_mutex);

    while(empty() && _flag) {
        _notEmpty.wait();
    }
    if(_flag) {
        ElemType ret = _que.front();
        _que.pop();
        _notFull.notify();

        return ret;
    }
    else {
        return nullptr;
    }
}

void TaskQueue::wakeup() {
    _flag = false;
    _notEmpty.notifyAll();
}

bool TaskQueue::empty() const {
    return 0 == _que.size();
}

bool TaskQueue::full() const {
    return _queSize == _que.size();
}