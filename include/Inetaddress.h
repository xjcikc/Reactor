#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <string>

using std::string;

class Inetaddress {
public:
    Inetaddress(const string &ip, unsigned short port);
    Inetaddress(const struct sockaddr_in &addr);
    ~Inetaddress();

    string ip() const;
    unsigned short port() const;

    const struct sockaddr_in* getsockaddr_inPtr() const; 
private:
    struct sockaddr_in _addr;
};

#endif