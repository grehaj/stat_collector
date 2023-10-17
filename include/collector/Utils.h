#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <string>

namespace collector
{
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

constexpr int READSIZE{1024};
constexpr storage_size_t MAX_STORAGE_SIZE{60};
constexpr file_count_t MAX_FILE_COUNT{60};
const std::string LOG_TEMPLATE{"stat_%d.json"};
const std::string DEFAULT_LOG_LOCATION{"/tmp/stat_%d.json"};

ip_t str_to_ip(const std::string&);
std::string ip_to_str(ip_t);
port_t str_to_port(const std::string&);
std::string port_to_str(port_t);
std::map<std::string, std::string> get_active_interfaces_ip();

std::string to_string(time_t t);

template<typename F>
struct fifo_deleter
{
    void operator ()(F* f)
    {
        if(f)
            pclose(f);
    }
};
}
}
