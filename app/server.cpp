#include "network/SocketConsts.h"
#include "network/UnixStreamServer.h"

using namespace std;

int main(int argv, char* argc[])
{
    network::UnixStreamServer server{network::UNIX_SOCKET_PATH};
    server.run();
    return 0;
}
