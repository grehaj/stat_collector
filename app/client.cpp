#include "network/SocketConsts.h"
#include "network/UnixStreamClient.h"
#include <iostream>

using namespace std;

int main(int argv, char* argc[])
{
    network::UnixStreamClient client{network::UNIX_SOCKET_PATH};
    client.run();
    return 0;
}
