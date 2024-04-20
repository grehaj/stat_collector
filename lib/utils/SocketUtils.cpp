#include "SocketUtils.h"
#include "Utils.h"

#include <algorithm>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace utils
{
sockaddr_in get_socket_address()
{
    sockaddr_in add{};

    return add;
}
}
