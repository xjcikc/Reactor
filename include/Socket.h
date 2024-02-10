#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "Noncopyable.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

class Socket: Noncopyable {
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();

    int fd() const;
    void shutDownWrite();
private:
    int _fd;
};

#endif