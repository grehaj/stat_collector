#include "network/ClientSocket.h"
#include "collector/Collector.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network
{

ClientSocket::ClientSocket(int domain, int type) : socket_id{socket(domain, type, 0)}
{
//    std::filesystem::remove(std::filesystem::path{collector::COLLECTOR_SOCKET_PTH});
//    if(socket_id == -1)
//    {
//        throw std::runtime_error{"server - socket"};
//    }

//    auto add{get_socket_address()};

//    if(bind(socket_id, reinterpret_cast<sockaddr*>(&add), sizeof (sockaddr_un)) == -1)
//    {
//        throw std::runtime_error{"server - bind"};
//    }

//    if(listen(socket_id, 10) == -1)
//    {
//        throw std::runtime_error{"server - listen"};
//    }
}

void ClientSocket::run()
{
    collector::Collector{"wlp2s0", 3, 10}.run();
}

}
