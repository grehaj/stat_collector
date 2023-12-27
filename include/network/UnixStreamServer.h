#pragma once

#include <string>
#include <sys/socket.h>

namespace network
{
class UnixStreamServer
{
public:
    UnixStreamServer(const std::string& addr);
    ~UnixStreamServer();

    void run() const;

private:
    const std::string address;
    const int socket_id{-1};
};
}
