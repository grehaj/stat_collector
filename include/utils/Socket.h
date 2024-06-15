#pragma once

#include "ISocket.h"

namespace utils
{
class Socket: public ISocket
{
public:
    int socket(int domain, int type, int protocol) override;
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) override;
    int listen(int sockfd, int backlog) override;
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) override;
    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) override;
    ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *src_addr, socklen_t *addrlen) override;
    ssize_t sendto(int sockfd, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) override;
    ssize_t read(int fd, void *buffer, size_t count) override;
    ssize_t write(int fd, void *buffer, size_t count) override;
    int getaddrinfo(const char *host, const char *service, const struct addrinfo *hints, struct addrinfo **result) override;
    void freeaddrinfo(struct addrinfo *res) override;
    std::string readLine(int sockfd) override;
    int close(int sockfd) override;
};
}
