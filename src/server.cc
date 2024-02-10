#include "Acceptor.h"
#include "TcpConnection.h"
#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

void server() {
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready();

    TcpConnection connection(acceptor.accept());

    cout << connection.toString() << "has connected" << endl;

    while(1) {
        cout << ">>recv msg from client: " <<connection.receive() << endl;
        connection.mysend("hello\n");
    }
}

int main()
{
    server();

    return 0;
}