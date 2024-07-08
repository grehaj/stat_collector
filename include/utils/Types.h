#pragma once

#include <cstdint>
#include <ctime>

namespace utils
{
struct port_t
{
    uint16_t port;
    bool operator<(const port_t& r) const;
};

struct ip_t
{
    uint32_t ip;
    bool operator<(const ip_t& r) const;
};

struct counter_t
{
    uint64_t counter;
    counter_t& operator++();
};

using file_count_t = uint8_t;
using storage_size_t = uint8_t;

struct traffic_t
{
    time_t t;
    utils::ip_t src_ip;
    utils::port_t src_port;
    utils::ip_t dst_ip;
    utils::port_t dst_port;
};
}
