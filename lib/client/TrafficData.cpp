#include "TrafficData.h"
#include "Utils.h"

#include <iomanip>

namespace collector
{
bool Socket::operator<(const Socket& r) const
{
    if(ip.ip != r.ip.ip)
        return ip < r.ip;

    return port.port < r.port.port;
}

PortTraffic& PortTraffic::operator+=(const Connection& pt)
{
    SocketTraffic& elem = amount_per_dest_port[pt.dst.port];
    ++elem.per_socket_count[pt.src];
    ++elem.total_count;
    ++total_count;

    return *this;
}
}
