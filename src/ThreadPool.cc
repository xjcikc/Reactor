#include "ThreadPool.h"
#include "Thread.h"

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool() {}

void ThreadPool::start() {
    for(size_t i = 0; i < _threadNum; ++i) {
        unique_ptr<Thread> newthread(new Thread(std::bind(&ThreadPool::doTask, this)));
        newthread->start();
        _threads.push_back(std::move(newthread));// unique_ptr不能复制和赋值，所以用移动语义
    }
}

void ThreadPool::stop() {
    // 防止任务没执行完就退出
    while(!_taskQue.empty()) {
        sleep(1);
    }
    
    _isExit = true;
    // 将所有等待_notEmpty的线程唤醒，从而进行回收
    _taskQue.wakeup();

    for(auto &th : _threads) {
        th->stop();
    }
}

void ThreadPool::addTask(Task &&task) {
    if(task) {
        _taskQue.push(std::move(task));
    }
}

void ThreadPool::doTask() {
    while(!_isExit) {
        Task task = getTask();

        if(task) {
            task();
        }
    }
}

Task ThreadPool::getTask() {
    return _taskQue.pop();
}