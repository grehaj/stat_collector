#include "SocketUtils.h"
#include "Utils.h"

#include <algorithm>

#include <sys/socket.h>
#include <sys/un.h>

namespace utils
{
sockaddr_un get_socket_address()
{
    sockaddr_un add{};
    add.sun_family = AF_UNIX;
    std::copy(std::begin(COLLECTOR_SOCKET_PTH), std::end(COLLECTOR_SOCKET_PTH), std::begin(add.sun_path));

    return add;
}
}
