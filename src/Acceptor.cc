#include "Acceptor.h"

Acceptor::Acceptor(const string &ip, unsigned short port): _sock(), _addr(ip, port) {}

Acceptor::~Acceptor() {}

void Acceptor::ready() {
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}

void Acceptor::setReuseAddr() {
    int opt = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(-1 == ret) {
        perror("setsockopt ip error");
        return;
    }
}

void Acceptor::setReusePort() {
    int opt = 1;
    int ret = setsockopt(_sock.fd(), SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if(-1 == ret) {
        perror("setsockopt port error");
        return;
    }
}

void Acceptor::bind() {
    socklen_t length = sizeof(struct sockaddr);
    if(::bind(_sock.fd(), (struct sockaddr*)_addr.getsockaddr_inPtr(), length) < 0) {
        perror("listen");
        return;
    }
}

void Acceptor::listen() {
    if(::listen(_sock.fd(), 128) < 0) {
        perror("listen");
        return;
    }
}

int Acceptor::accept() {
    int connfd = ::accept(_sock.fd(), nullptr, nullptr);
    if(-1 == connfd) {
        perror("accept");
        return -1;
    }
    return connfd;
}

int Acceptor::fd() const {
    return _sock.fd();
}