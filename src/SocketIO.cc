#include "SocketIO.h"

SocketIO::SocketIO(int fd): _fd(fd) {}

SocketIO::~SocketIO() {}

int SocketIO::readn(char* buf, int len) {
    int n = len;
    char* p = buf;
    int ret = 0;
    while(n > 0) {
        ret = read(_fd, p, n);
        if(-1 == ret && errno == EINTR) {
            continue;
        }
        else if(-1 == ret) {
            perror("read error -1");
            return ret;
        }
        else if(0 == ret) {
            break;
        }
        else {
            n -= ret;
            p += ret;
        }
    }
    return len - n;
}

int SocketIO::writen(const char* buf, int len) {
    int n = len;
    const char* p = buf;
    int ret = 0;
    while(n > 0) {
        ret = write(_fd, p, n);
        if(-1 == ret && errno == EINTR) {
            continue;
        }
        else if(-1 == ret) {
            perror("write error -1");
            return -1;
        }
        else if(0 == ret) {
            break;
        }
        else {
            n -= ret;
            p += ret;
        }
    }
    return len - n;
}

int SocketIO::readLine(char* buf, int len) {
    int n = len - 1;
    char* p = buf;
    int ret = 0, total = 0;
    while(n > 0) {
        ret = recv(_fd, p, n, MSG_PEEK); //***
        if(-1 == ret && errno == EINTR) {
            continue;
        }
        else if(-1 == ret) {
            perror("readLine error -1");
            return ret;
        }
        else if(0 == ret) {
            break;
        }
        else {
            for(int idx = 0; idx < ret; ++idx) {
                if(p[idx] == '\n') {
                    int sz = idx + 1;
                    readn(p, sz);
                    p += sz;
                    *p = '\0';

                    return total + sz;
                }
            }
            readn(p, ret);
            total += ret;
            p += ret;
            n -= ret;
        }
    }
    *p = '\0';

    return total - n;
}