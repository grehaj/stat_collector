#include "network/UnixStreamClient.h"
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
UnixStreamClient::UnixStreamClient(const std::string& addr): address{addr}, socket_id{socket(AF_UNIX, SOCK_STREAM, 0)}
{
    if(socket_id == -1)
    {
        throw std::runtime_error{"server - socket"};
    }

    if(address.size() >= sizeof(sockaddr_un{}.sun_path))
    {
        throw std::runtime_error{"socket - address."};
    }
}

UnixStreamClient::~UnixStreamClient()
{
    ::close(socket_id);
}

void UnixStreamClient::run() const
{
    using namespace std::chrono_literals;
    using namespace std::string_literals;

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;

    std::copy(address.begin(), address.end(), std::begin(addr.sun_path));

    if(::connect(socket_id, reinterpret_cast<sockaddr*>(&addr), sizeof (sockaddr_un)) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        throw std::runtime_error{"Unable to connect unix socket."};
    }

    const std::string msg = "Hello World!!!";
    // make sure it does not overflow
    const ssize_t msg_length = msg.length();
    const auto wrote = ::write(socket_id, msg.c_str(), msg_length);

    if(wrote != msg_length)
    {
        throw std::runtime_error{"Unable to write unix socket."};
    }
    std::this_thread::sleep_for(1s);
}
}
