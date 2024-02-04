#include "server/StatServer.h"

#include <sys/un.h>
#include <sys/socket.h>

int main(int argv, char* argc[])
{
    server::StatServer server{};
    server.run();
    return 0;
}
