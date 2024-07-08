#include "Utils.h"
#include "Consts.h"

#include <algorithm>
#include <arpa/inet.h>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>

//debug
#include <iostream>

namespace utils
{

bool port_t::operator<(const port_t& r) const
{
    return port < r.port;
}

bool ip_t::operator<(const ip_t& r) const
{
    return ip < r.ip;
}

counter_t& counter_t::operator++()
{
    ++counter;
    return *this;
}

ip_t str_to_ip(const std::string& ip)
{
    in_addr add;

    inet_pton(AF_INET, ip.c_str(), &(add));

    ip_t result;
    result.ip = add.s_addr;
    return result;
}

std::string ip_to_str(ip_t ip)
{
    in_addr add{ip.ip};
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(add), str, INET_ADDRSTRLEN);

    return std::string(str);
}

port_t str_to_port(const std::string& port)
{
    port_t p;
    p.port = std::stoi(port.c_str());
    return p;
}
std::string port_to_str(port_t port)
{
    return std::to_string(ntohs(port.port));
}

std::string readLine(int fd)
{
    ssize_t numRead{};
    std::string result;
    char ch{};

    while( true )
    {
        numRead = read(fd, &ch, 1);
        if (numRead == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                throw std::runtime_error{std::string{"readLine:" } + strerror(errno)};
            }
        } else if (numRead == 0)
        {
            break;
        }
        else
        {
            result += ch;
            if (ch == '\n')
            {
                break;
            }
        }
    }

    return result;
}
}
