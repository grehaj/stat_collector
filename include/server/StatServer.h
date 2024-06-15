#pragma once

#include "IServerRunGuard.h"
#include "ISocket.h"

#include <memory>

namespace server
{

class StatServer
{
public:
    StatServer(std::unique_ptr<utils::ISocket> soc, std::unique_ptr<IServerRunGuard> server_guard);

    void run();

private:
    std::unique_ptr<utils::ISocket> soc;
    std::unique_ptr<IServerRunGuard> server_guard;
    int socket_id{-1};
};

}
