#include "TcpConnection.h"
#include "EventLoop.h"
#include <sstream>
#include <iostream>

using std::endl;
using std::cout;

TcpConnection::TcpConnection(int fd, EventLoop* loop)
: _socketIO(fd)
, _sock(fd)
, _loop(loop)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr()) {}

TcpConnection::~TcpConnection() {
    // debug
    cout << "~TcpConnection()" << endl;
}

void TcpConnection::mysend(const string &msg) {
    _socketIO.writen(msg.c_str(), msg.size());
}

void TcpConnection::sendInLoop(const string &msg) {
    if(_loop) {
        _loop->runInLoop(std::bind(&TcpConnection::mysend, this, msg));
    }
}

string TcpConnection::receive() {
    char buff[65535] = {0};
    _socketIO.readLine(buff, sizeof(buff));

    return string(buff);
}

string TcpConnection::toString() {
    std::ostringstream oss;
    oss << _localAddr.ip() << ":"
        << _localAddr.port() << "-------->"
        << _peerAddr.ip() << ":"
        << _peerAddr.port();
    
    return oss.str();
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback &cb) {
    _onConnection = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback &cb) {
    _onMessage = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback &cb) {
    _onClose = cb;
}

void TcpConnection::handleConnectionCallback() {
    if(_onConnection) {
        _onConnection(shared_from_this());
    }
}

void TcpConnection::handleMessageCallback() {
    if(_onMessage) {
        _onMessage(shared_from_this());
    }
}

void TcpConnection::handleCloseCallback() {
    if(_onClose) {
        _onClose(shared_from_this());
    }
}

bool TcpConnection::isClosed() const {
    int ret = 0;
    do {
        char buff[64] = {0};
        ret = recv(_sock.fd(), buff, sizeof(buff), MSG_PEEK);
    } while(ret == -1 && errno == EINTR);
    return ret == 0;
}

Inetaddress TcpConnection::getLocalAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);

    int ret = getsockname(_sock.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret) {
        perror("getsockname");
    }
    return Inetaddress(addr);
}

Inetaddress TcpConnection::getPeerAddr() {
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);

    int ret = getpeername(_sock.fd(), (struct sockaddr*)&addr, &len);
    if(-1 == ret) {
        perror("getpeername");
    }
    return Inetaddress(addr);
}