#include "Acceptor.h"
#include "TcpConnection.h"
#include "EventLoop.h"
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

void server() {
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();

    EventLoop loop(acceptor);
    loop.setAllCallbacks(onConnection, onMessage, onClose);
    loop.loop();
}

int main()
{
    server();

    return 0;
}