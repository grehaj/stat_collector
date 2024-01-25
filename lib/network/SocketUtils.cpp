#include "network/SocketUtils.h"
#include "collector/Utils.h"

#include <algorithm>

#include <sys/socket.h>
#include <sys/un.h>

namespace network
{
sockaddr_un get_socket_address()
{
    sockaddr_un add{};
    add.sun_family = AF_UNIX;
    std::copy(std::begin(collector::COLLECTOR_SOCKET_PTH), std::end(collector::COLLECTOR_SOCKET_PTH), std::begin(add.sun_path));

    return add;
}
}
