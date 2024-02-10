#include "TcpConnection.h"
#include <sstream>

TcpConnection::TcpConnection(int fd)
: _socketIO(fd), _sock(fd), _localAddr(getLocalAddr()), _peerAddr(getPeerAddr()) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::mysend(const string &msg) {
    _socketIO.writen(msg.c_str(), msg.size());
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