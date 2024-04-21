#pragma once

#include "Utils.h"
#include <ctime>

namespace client
{
struct TrafficData
{
    time_t t;
    utils::ip_t src_ip;
    utils::port_t src_port;
    utils::ip_t dst_ip;
    utils::port_t dst_port;
};
}
