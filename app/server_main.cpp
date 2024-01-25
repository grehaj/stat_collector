#include "server/StatServer.h"

#include <sys/un.h>
#include <sys/socket.h>

using namespace network;

int main(int argv, char* argc[])
{
    StatServer server{};
    server.run();
    return 0;
}
