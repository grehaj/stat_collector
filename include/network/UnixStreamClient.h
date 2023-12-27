#pragma once

#include <string>
#include <sys/socket.h>

namespace network
{
class UnixStreamClient
{
public:
    UnixStreamClient(const std::string& addr);
    ~UnixStreamClient();

    void run() const;

private:
    const std::string address;
    const int socket_id{-1};
};
}
