#pragma once

#include <string>

namespace network
{
constexpr int BACKLOG{5};
const std::string UNIX_SOCKET_PATH{"/tmp/unix_stream_socket"};
}
