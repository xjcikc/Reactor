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

class EventLoop;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
, Noncopyable {
public:
    explicit TcpConnection(int fd, EventLoop* loop);
    ~TcpConnection();

    void mysend(const string &msg);
    void sendInLoop(const string &msg);
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
    Socket _sock;
    EventLoop* _loop;
    bool _shutdownWrite;
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;
    //debug
    Inetaddress _localAddr;
    Inetaddress _peerAddr;
};

#endif