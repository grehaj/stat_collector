#pragma once

namespace server
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
