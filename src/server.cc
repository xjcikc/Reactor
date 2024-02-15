#include "TcpServer.h"
#include "ThreadPool.h"
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

class MyTask {
public:
    MyTask(const string &msg, const TcpConnectionPtr &connection)
    : _msg(msg), _connection(connection) {}

    void process() {
        cout << "receive from client: " << _msg << endl;
        cout << "process(): " << pthread_self() << endl;
        _msg = "after process: " + _msg;
        _connection->sendInLoop(_msg);
    }
private:
    string _msg;
    TcpConnectionPtr _connection;
};

class EchoServer {
public:
    EchoServer(const string &ip, unsigned short port)
    : _threadpool(4, 20)
    , _server(ip, port) {
        _server.setAllCallbacks(std::bind(&EchoServer::onConnection, this, std::placeholders::_1),
                                std::bind(&EchoServer::onMessage, this, std::placeholders::_1),
                                std::bind(&EchoServer::onClose, this, std::placeholders::_1));
    }
    void onConnection(const TcpConnectionPtr &connection) {
        cout << connection->toString() << " has connected!" << endl;
    }

    void onMessage(const TcpConnectionPtr &connection) {
        string msg = connection->receive();
        // 将消息封装成一个任务，交给线程池去处理
        MyTask task(msg, connection);
        _threadpool.addTask(std::bind(&MyTask::process, &task));
    }

    void onClose(const TcpConnectionPtr &connection) {
        cout << connection->toString() << " has closed!" << endl;
    }

    void start() {
        _threadpool.start();
        _server.start();
    }
private:
    ThreadPool _threadpool;
    TcpServer _server;
};

void server2() {
    EchoServer server("127.0.0.1", 8888);
    server.start();
}

int main()
{
    server2();

    return 0;
}