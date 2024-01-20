#include "collector/Utils.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace collector;
using namespace std;

int main(int argv, char* argc[])
{
    ssize_t bytes_read{-1};
    char buffer[utils::READSIZE]{};
    sockaddr_un add{};
    add.sun_family = AF_UNIX;
    std::copy(std::begin(utils::COLLECTOR_SOCKET_PTH), std::end(utils::COLLECTOR_SOCKET_PTH), std::begin(add.sun_path));
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    std::filesystem::remove(std::filesystem::path{utils::COLLECTOR_SOCKET_PTH});
    if(server_socket == -1)
    {
        throw std::runtime_error{"server - socket"};
    }

    if(bind(server_socket, reinterpret_cast<sockaddr*>(&add), sizeof (sockaddr_un)) == -1)
    {
        throw std::runtime_error{"server - bind"};
    }

    if(listen(server_socket, 10) == -1)
    {
        throw std::runtime_error{"server - listen"};
    }

    while(true)
    {
        auto client_fd = accept(server_socket, nullptr, nullptr);
        if (client_fd == -1)
        {
            throw std::runtime_error{"server - accept"};
        }

        while((bytes_read = read(client_fd, buffer, utils::READSIZE)) > 0)
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
    return 0;
}
