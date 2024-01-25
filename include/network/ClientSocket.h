#pragma once

#include "network/ISocket.h"

namespace network
{

class ClientSocket : public ISocket
{
public:
    ClientSocket(int domain, int type);

    void run() override;

private:
    int socket_id{-1};
};

}
