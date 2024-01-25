#include "network/ServerSocket.h"

#include <sys/un.h>
#include <sys/socket.h>

using namespace network;

int main(int argv, char* argc[])
{
    ServerSocket socket{AF_UNIX, SOCK_STREAM};
    socket.run();
    return 0;
}
