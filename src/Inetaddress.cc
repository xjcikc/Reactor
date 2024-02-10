#include "Inetaddress.h"

Inetaddress::Inetaddress(const string &ip, unsigned short port) {
    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

Inetaddress::Inetaddress(const struct sockaddr_in &addr): _addr(addr) {}

Inetaddress::~Inetaddress() {}

string Inetaddress::ip() const {
    return inet_ntoa(_addr.sin_addr);
}

unsigned short Inetaddress::port() const {
    return ntohs(_addr.sin_port);
}

const struct sockaddr_in* Inetaddress::getsockaddr_inPtr() const {
    return &_addr;
}