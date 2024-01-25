#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <string>

namespace collector
// TODO types
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


// TODO consts
constexpr int READSIZE{1024};
constexpr storage_size_t MAX_STORAGE_SIZE{60};
constexpr file_count_t MAX_FILE_COUNT{60};
const std::string COLLECTOR_SOCKET_PTH{"/tmp/collector_socket"};


//TODO split utils into network utils ...
ip_t str_to_ip(const std::string&);
std::string ip_to_str(ip_t);
port_t str_to_port(const std::string&);
std::string port_to_str(port_t);
std::map<std::string, std::string> get_active_interfaces_ip();

//TODO time utils
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

//TODO network utils and logic to avoid overflow and client id for client identification
struct UdpMsg
{
    char data[READSIZE];
};
}

