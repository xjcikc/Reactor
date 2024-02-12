#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "SocketIO.h"
#include "Inetaddress.h"
#include "Socket.h"
#include <string>
#include <functional>
#include <memory>

using std::string;
using std::function;
using std::shared_ptr;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
, Noncopyable {
public:
    explicit TcpConnection(int fd);
    ~TcpConnection();

    void mysend(const string &msg);
    string receive();
    //debug
    string toString();

    void setConnectionCallback(const TcpConnectionCallback &);
    void setMessageCallback(const TcpConnectionCallback &);
    void setCloseCallback(const TcpConnectionCallback &);
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

    bool isClosed() const;
private:
    //debug
    Inetaddress getLocalAddr();
    Inetaddress getPeerAddr();
private:
    SocketIO _socketIO;
    bool _shutdownWrite;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    Socket _sock;
    //debug
    Inetaddress _localAddr;
    Inetaddress _peerAddr;
};

#endif