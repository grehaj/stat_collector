#pragma once

#include "Types.h"

#include <ctime>
#include <map>
#include <string>

namespace utils
{
ip_t str_to_ip(const std::string&);
std::string ip_to_str(ip_t);
port_t str_to_port(const std::string&);
std::string port_to_str(port_t);
std::map<std::string, std::string> get_active_interfaces_ip();

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

