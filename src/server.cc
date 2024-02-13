#include "TcpServer.h"
#include <iostream>

using std::cout;
using std::endl;

void onConnection(const TcpConnectionPtr &connection) {
    cout << connection->toString() << " has connected!" << endl;
}

void onMessage(const TcpConnectionPtr &connection) {
    string msg = connection->receive();
    cout << "receive from client: " << msg << endl;

    connection->mysend(msg);
}

void onClose(const TcpConnectionPtr &connection) {
    cout << connection->toString() << " has closed!" << endl;
}

void server1() {
    TcpServer server("127.0.0.1", 8888);
    server.setAllCallbacks(onConnection, onMessage, onClose);
    server.start();
}

class EchoServer {
public:
    EchoServer(const string &ip, unsigned short port): _server(ip, port) {
        _server.setAllCallbacks(std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
                                std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
                                std::bind(&EchoServer::onClose, this, std::placeholders::_1));
    }
    void onConnection(const TcpConnectionPtr &connection) {
        cout << connection->toString() << " has connected!" << endl;
    }

    void onMessage(const TcpConnectionPtr &connection) {
        string msg = connection->receive();
        cout << "receive from client: " << msg << endl;

        connection->mysend(msg);
    }

    void onClose(const TcpConnectionPtr &connection) {
        cout << connection->toString() << " has closed!" << endl;
    }

    void start() {
        _server.start();
    }
private:
    TcpServer _server;
};

void server2() {
    EchoServer server("127.0.0.1", 8888);
    server.start();
}

int main()
{
    server1();

    return 0;
}