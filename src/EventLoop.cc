#include "EventLoop.h"
#include "Acceptor.h"
#include <sys/epoll.h>

#define EPOLL_EVENT_SIZE 1024

EventLoop::EventLoop(Acceptor &acceptor)
: _epfd(createEpollFd()), _acceptor(acceptor), _isLooping(true), _evtList(EPOLL_EVENT_SIZE) {
    epollAddReadFd(_acceptor.fd());
}

EventLoop::~EventLoop() {
    close(_epfd);
}

void EventLoop::loop() {
    while(_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::unloop() {
    _isLooping = false;
}

void EventLoop::waitEpollFd() {
    int nready;
    do {
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);
    } while(nready == -1 && errno == EINTR);

    if(nready == -1) {
        perror("epoll_wait");
    }
    else if(nready == 0) {
        printf("epoll_wait timeout\n");
    }
    else{
        if(nready == _evtList.size()) {
            _evtList.resize(2 * nready);
        }
        
        for(int i = 0; i < nready; ++i) {
            int fd = _evtList[i].data.fd;
            if(fd == _acceptor.fd()) {
                handleNewConnection();
            } 
            else {
                handleMessage(fd);
            }
        }
    }
}

void EventLoop::setAllCallbacks(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3) {
    _onConnection = std::move(cb1);
    _onMessage = std::move(cb2);
    _onClose = std::move(cb3);
}

void EventLoop::handleNewConnection() {
    int peerfd = _acceptor.accept();
    epollAddReadFd(peerfd);
    TcpConnectionPtr connection(new TcpConnection(peerfd));
    connection->setConnectionCallback(_onConnection);
    connection->setMessageCallback(_onMessage);
    connection->setCloseCallback(_onClose);

    _connectionMap.insert(std::make_pair(peerfd, connection));

    connection->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd) {
    auto it = _connectionMap.find(fd);
    if(it != _connectionMap.end()) {
        auto connection = it->second;
        if(connection->isClosed()) {
            connection->handleCloseCallback();
            epollDelReadFd(fd);
            _connectionMap.erase(fd);
        }
        else {
            connection->handleMessageCallback();
        }
    }
}

int EventLoop::createEpollFd() {
    int fd = ::epoll_create1(0);
    if(fd < 0) {
        perror("epoll_create1");
    }
    return fd;
}

void EventLoop::epollAddReadFd(int fd) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
    if(ret < 0) {
        perror("epoll_ctl_add");
    }
}

void EventLoop::epollDelReadFd(int fd) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = fd;

    int ret = ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
    if(ret < 0) {
        perror("epoll_ctl_del");
    }
}