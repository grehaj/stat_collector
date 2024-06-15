#include "Socket.h"
#include "Utils.h"

#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>

namespace utils
{

int Socket::socket(int domain, int type, int protocol)
{
    return ::socket(domain, type, protocol);
}

int Socket::bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return ::bind(sockfd, addr, addrlen);
}

int Socket::listen(int sockfd, int backlog)
{
    return ::listen(sockfd, backlog);
}

int Socket::accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return ::accept(sockfd, addr, addrlen);
}

int Socket::connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return ::connect(sockfd, addr, addrlen);
}

ssize_t Socket::recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
{
    return ::recvfrom(sockfd, buffer, length, flags, src_addr, addrlen);
}

ssize_t Socket::sendto(int sockfd, const void *buffer, size_t length, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
{
    return ::sendto(sockfd, buffer, length, flags, dest_addr, addrlen);
}

ssize_t Socket::read(int fd, void *buffer, size_t count)
{
    return ::read(fd, buffer, count);
}

ssize_t Socket::write(int fd, void *buffer, size_t count)
{
    return ::write(fd, buffer, count);
}

int Socket::getaddrinfo(const char *host, const char *service, const struct addrinfo *hints, struct addrinfo **result)
{
    return ::getaddrinfo(host, service, hints, result);
}

void Socket::freeaddrinfo(struct addrinfo *res)
{
    return ::freeaddrinfo(res);
}

std::string Socket::readLine(int sockfd)
{
    return utils::readLine(sockfd);
}

int Socket::close(int sockfd)
{
    return ::close(sockfd);
}

}
