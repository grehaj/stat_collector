#pragma once

#include "network/ISocket.h"

namespace network
{

class ServerSocket : public ISocket
{
public:
    ServerSocket(int domain, int type);

    void run() override;

private:
    int socket_id{-1};
};

}
