#pragma once

namespace network
{

class StatServer
{
public:
    StatServer();

    void run();

private:
    int socket_id{-1};
};

}
