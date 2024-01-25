#include "server/StatServer.h"
#include "utils/Utils.h"
#include "utils/SocketUtils.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network
{

StatServer::StatServer() : socket_id{socket(AF_UNIX, SOCK_DGRAM, 0)}
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
}

void StatServer::run()
{
    ssize_t bytes_read{-1};
    char buffer[collector::READSIZE]{};
    while(true)
    {
        //for udp we need a thread for each client identified by client id
        bytes_read = recvfrom(socket_id, buffer, collector::READSIZE, 0, 0, 0);
        while(bytes_read == -1)
        {
            throw std::runtime_error{"server - recvfrom"};
        }

        std::cout << buffer << std::endl;
        std::fill(std::begin(buffer), std::end(buffer), 0);

        // TODO need a suprt for multiple clients sending data
    }
}

}
