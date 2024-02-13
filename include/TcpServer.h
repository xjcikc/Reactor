#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Acceptor.h"
#include "EventLoop.h"
#include "TcpConnection.h"

class TcpServer {
public:
    TcpServer(const string &ip, unsigned short port);
    ~TcpServer();

    void start();
    void stop();
    void setAllCallbacks(TcpConnectionCallback &&cb1, TcpConnectionCallback &&cb2, TcpConnectionCallback &&cb3);
private:
    Acceptor _acceptor;
    EventLoop _loop;
};

#endif