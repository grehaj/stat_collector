#include "network/ServerSocket.h"
#include "collector/Utils.h"
#include "network/SocketUtils.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network
{

ServerSocket::ServerSocket(int domain, int type) : socket_id{socket(domain, type, 0)}
{
    std::filesystem::remove(std::filesystem::path{collector::COLLECTOR_SOCKET_PTH});
    if(socket_id == -1)
    {
        throw std::runtime_error{"server - socket"};
    }

    auto add{get_socket_address()};

    if(bind(socket_id, reinterpret_cast<sockaddr*>(&add), sizeof (sockaddr_un)) == -1)
    {
        throw std::runtime_error{"server - bind"};
    }

    if(listen(socket_id, 10) == -1)
    {
        throw std::runtime_error{"server - listen"};
    }
}

void ServerSocket::run()
{
    ssize_t bytes_read{-1};
    char buffer[collector::READSIZE]{};
    while(true)
    {
        const auto client_fd = accept(socket_id, nullptr, nullptr);
        if (client_fd == -1)
        {
            throw std::runtime_error{"server - accept"};
        }

        while((bytes_read = read(client_fd, buffer, collector::READSIZE)) > 0)
        {
            std::cout << buffer;
            std::fill(std::begin(buffer), std::end(buffer), 0);
        }

        if(bytes_read < 0)
        {
            throw std::runtime_error{"server - read"};
        }

        if(close(client_fd) == -1)
        {
            throw std::runtime_error{"server - close"};
        }
        // TODO need a suprt for multiple clients sending data
    }
}

}
