#pragma once

#include "collector/Utils.h"

#include <algorithm>

#include <sys/socket.h>
#include <sys/un.h>

namespace network
{

sockaddr_un get_socket_address();

}

