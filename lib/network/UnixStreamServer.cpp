#include "network/UnixStreamServer.h"
#include "network/SocketConsts.h"

#include <chrono>
#include <cerrno>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


namespace network
{
UnixStreamServer::UnixStreamServer(const std::string& addr): address{addr}, socket_id{socket(AF_UNIX, SOCK_STREAM, 0)}
{
    std::filesystem::remove(std::filesystem::path{address});
    if(socket_id == -1)
    {
        throw std::runtime_error{"server - socket"};
    }

    if(address.size() >= sizeof(sockaddr_un{}.sun_path))
    {
        throw std::runtime_error{"socket - address."};
    }
}

UnixStreamServer::~UnixStreamServer()
{
    std::filesystem::remove(std::filesystem::path{address});
    ::close(socket_id);
}

void UnixStreamServer::run() const
{
    using namespace std::chrono_literals;
    using namespace std::string_literals;
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    const auto max_path_len = sizeof(addr.sun_path) - 1;

    std::copy(address.begin(), address.end(), std::begin(addr.sun_path));

    if(::bind(socket_id, reinterpret_cast<sockaddr*>(&addr), max_path_len) == -1)
    {
        throw std::runtime_error{"server - bind"};
    }

    if(::listen(socket_id, BACKLOG) == -1)
    {
        throw std::runtime_error{"server - listen"};
    }

    while(true)
    {
        int peer_socket = ::accept(socket_id, nullptr, nullptr);
        if(peer_socket == -1)
        {
            throw std::runtime_error{"server - accept"};
        }

        ssize_t bytes_read{-1};
        char buffer[100];
        while((bytes_read = ::read(peer_socket, buffer, 100)) > 0)
        {
            if(bytes_read == 100)
            {
                buffer[bytes_read - 1] = '\0';
            }

            std::this_thread::sleep_for(1s);
            std::cout << "Server received: " << buffer << std::endl;
        }

        if(bytes_read == -1)
        {
            throw std::runtime_error{"server - read"};
        }
        else if(bytes_read == 0)
        {
            break;
        }

        if(::close(peer_socket) == -1)
        {
            throw std::runtime_error{"server - close"};
        }
    }
}
}
