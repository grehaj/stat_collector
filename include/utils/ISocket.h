#pragma once

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace utils
{
class ISocket
{
public:
    virtual ~ISocket() = default;

    virtual int socket(int domain, int type, int protocol) = 0;
    virtual int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;
    virtual int listen(int sockfd, int backlog) = 0;
    virtual int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) = 0;
    virtual int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = 0;
    virtual ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *src_addr, socklen_t *addrlen) = 0;
    virtual ssize_t sendto(int sockfd, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) = 0;
    virtual ssize_t read(int fd, void *buffer, size_t count) = 0;
    virtual ssize_t write(int fd, void *buffer, size_t count) = 0;
    virtual int getaddrinfo(const char *host, const char *service, const struct addrinfo *hints, struct addrinfo **result) = 0;
    virtual void freeaddrinfo(struct addrinfo *res) = 0;
    virtual std::string readLine(int sockfd) = 0;
    virtual int close(int sockfd) = 0;
};
}
