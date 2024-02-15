#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "TcpConnection.h"
#include "MutexLock.h"
#include <map>
#include <vector>

using std::map;
using std::vector;

class Acceptor;

using Functor = std::function<void()>;

class EventLoop {
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    void loop();
    void unloop();
    void runInLoop(Functor &&cb);
    void setAllCallbacks(TcpConnectionCallback &&,TcpConnectionCallback &&, TcpConnectionCallback &&);
private:
    void handleNewConnection();
    void handleMessage(int);
    void waitEpollFd();
    int createEpollFd();
    void epollAddReadFd(int);
    void epollDelReadFd(int);

    int createEventfd();
    void wakeupEventfd();
    void handleReadEventfd();
    void doPendingFunctors();
private:
    int _epfd;
    int _eventfd;
    Acceptor &_acceptor;
    bool _isLooping;
    vector<struct epoll_event> _evtList;
    map<int, TcpConnectionPtr> _connectionMap;
    MutexLock _mutex;
    vector<Functor> _pendingFunctors;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
};

#endif