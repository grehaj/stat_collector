#pragma once

namespace client
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
