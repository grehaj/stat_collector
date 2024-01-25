#pragma once

namespace network
{

class StatClient
{
public:
    StatClient();

    void run();

private:
    int socket_id{-1};
};

}
