#pragma once

#include "ISocket.h"

#include "gmock/gmock.h"

using namespace testing;

namespace utils
{

class SocketMock : public ISocket
{
public:
    MOCK_METHOD3(socket, int(int, int, int));
    MOCK_METHOD3(bind, int(int, const sockaddr*, socklen_t));
    MOCK_METHOD2(listen, int(int, int));
    MOCK_METHOD3(accept, int(int, sockaddr*, socklen_t*));
    MOCK_METHOD3(connect, int(int, const sockaddr*, socklen_t));
    MOCK_METHOD6(recvfrom, ssize_t(int, void*, size_t, int, sockaddr* , socklen_t*));
    MOCK_METHOD6(sendto, ssize_t(int, const void*, size_t, int, const sockaddr* , socklen_t));
    MOCK_METHOD3(read, ssize_t(int, void*, size_t));
    MOCK_METHOD3(write, ssize_t(int, void*, size_t));
    MOCK_METHOD4(getaddrinfo, int(const char*, const char*, const addrinfo*, addrinfo**));
    MOCK_METHOD1(freeaddrinfo, void(addrinfo*));
    MOCK_METHOD1(readLine, std::string(int));
    MOCK_METHOD1(close, int(int));
};


}
