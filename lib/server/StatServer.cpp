#include "server/StatServer.h"
#include "Consts.h"
#include "utils/Utils.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

namespace server
{

StatServer::StatServer(std::unique_ptr<utils::ISocket> socInit, std::unique_ptr<IServerRunGuard> server_guard_init):
    soc{std::move(socInit)}, server_guard{std::move(server_guard_init)}
{
    addrinfo hints{};
    addrinfo *result{};
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    /* Allows IPv4 or IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    /* Wildcard IP address; service name is numeric */
    if (soc->getaddrinfo(NULL, utils::PORT_NUM.c_str(), &hints, &result) != 0)
    {
        throw std::runtime_error{"server - getaddrinfo"};
    }
    auto rp = result;
    for(; rp; rp = rp->ai_next)
    {
        socket_id = soc->socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(socket_id == -1)
            continue;

//        if (setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval) == -1))
//        {
//            throw std::runtime_error{"server - setsockopt"};
//        }

        if (soc->bind(socket_id, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        soc->close(socket_id);
    }

    if (rp == NULL)
    {
        throw std::runtime_error{"server - Could not bind socket to any address"};
    }

    if (soc->listen(socket_id, utils::BACKLOG) == -1)
    {
        throw std::runtime_error{"server - listen"};
    }

    soc->freeaddrinfo(result);
}

void StatServer::run()
{
    sockaddr_storage claddr;
    while(not server_guard->should_stop())
    {
        socklen_t addrlen = sizeof(sockaddr_storage);
        auto cfd = soc->accept(socket_id, (struct sockaddr *) &claddr, &addrlen);
        if (cfd == -1)
        {
            std::cout << strerror(errno) << std::endl;
            continue;
        }
        auto msg = soc->readLine(cfd);
        while(!msg.empty())
        {
            std::cout << msg;
            msg = soc->readLine(cfd);
        }
        soc->close(cfd);
    }
}

}
