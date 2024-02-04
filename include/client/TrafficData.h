#pragma once

#include "Utils.h"
#include <ctime>
#include <cstdint>
#include <deque>
#include <map>
#include <ostream>
#include <string>

namespace client
{

struct Socket
{
    utils::ip_t ip;
    utils::port_t port;

    bool operator<(const Socket& r) const;
};

struct Connection
{
    Socket src;
    Socket dst;
};

std::ostream& operator<<(std::ostream& out, const Connection& s);

struct SocketTraffic
{
    std::map<Socket, utils::counter_t> per_socket_count;
    utils::counter_t total_count;
};

struct PortTraffic
{
    time_t observation_time;
    std::map<utils::port_t, SocketTraffic> amount_per_dest_port{};
    utils::counter_t total_count;

    PortTraffic& operator+=(const Connection& pt);
};

using traffic_t = std::deque<PortTraffic>;
}
