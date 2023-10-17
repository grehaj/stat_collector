#pragma once

#include "Utils.h"
#include <ctime>
#include <cstdint>
#include <deque>
#include <map>
#include <ostream>
#include <string>

namespace collector
{
using namespace utils;

struct Socket
{
    ip_t ip;
    port_t port;

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
    std::map<Socket, counter_t> per_socket_count;
    counter_t total_count;
};

struct PortTraffic
{
    time_t observation_time;
    std::map<port_t, SocketTraffic> amount_per_dest_port{};
    counter_t total_count;

    PortTraffic& operator+=(const Connection& pt);
};

using traffic_t = std::deque<PortTraffic>;
}
