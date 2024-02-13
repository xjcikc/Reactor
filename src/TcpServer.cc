#include "TcpServer.h"

TcpServer::TcpServer(const string &ip, unsigned short port): _acceptor(ip, port), _loop(_acceptor) {}

TcpServer::~TcpServer() {}

void TcpServer::start() {
    _acceptor.ready();
    _loop.loop();
}

void TcpServer::stop() {
    _loop.unloop();
}

void TcpServer::setAllCallbacks(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3) {
    _loop.setAllCallbacks(std::move(cb1), std::move(cb2), std::move(cb3));
}