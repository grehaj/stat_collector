#include "server/StatServer.h"
#include "server/ServerRunGuard.h"
#include "Socket.h"

#include <memory>
#include <sys/un.h>
#include <sys/socket.h>

int main(int argv, char* argc[])
{
    server::StatServer server{std::make_unique<utils::Socket>(), std::make_unique<server::ServerRunGuard>()};
    server.run();
    return 0;
}
