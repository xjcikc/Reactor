#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "TcpConnection.h"
#include <map>
#include <vector>

using std::map;
using std::vector;

class Acceptor;

class EventLoop {
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();

    void loop();
    void unloop();
    void setAllCallbacks(TcpConnectionCallback &&,TcpConnectionCallback &&, TcpConnectionCallback &&);
private:
    void handleNewConnection();
    void handleMessage(int);
    void waitEpollFd();
    int createEpollFd();
    void epollAddReadFd(int);
    void epollDelReadFd(int);
private:
    int _epfd;
    Acceptor &_acceptor;
    bool _isLooping;
    map<int, TcpConnectionPtr> _connectionMap;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    vector<struct epoll_event> _evtList;
};

#endif