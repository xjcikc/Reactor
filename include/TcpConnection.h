#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "SocketIO.h"
#include "Inetaddress.h"
#include "Socket.h"
#include <string>

using std::string;

class TcpConnection {
public:
    explicit TcpConnection(int fd);
    ~TcpConnection();

    void mysend(const string &msg);
    string receive();
    //debug
    string toString();
private:
    //debug
    Inetaddress getLocalAddr();
    Inetaddress getPeerAddr();
private:
    SocketIO _socketIO;
    //debug
    Socket _sock;
    Inetaddress _localAddr;
    Inetaddress _peerAddr;
};

#endif