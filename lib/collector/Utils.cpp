#include "Utils.h"
#include <algorithm>
#include <arpa/inet.h>
#include <iomanip>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>

namespace collector
{
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

std::map<std::string, std::string> get_active_interfaces_ip()
{
    FILE* f = popen("ip -4 addr", "r");
    if(f == nullptr)
        throw std::runtime_error{"SystemCommand: popen - ip addr"};

    std::regex r{R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"};
    std::smatch smtch;
    std::map<std::string, std::string> active_interfaces_ip;
    char buffer[READSIZE]{};

    while (fgets(buffer, READSIZE, f))
    {
        std::string s{buffer};
        if(std::regex_search(s, smtch, r))
        {
            auto start = s.find_last_of(" ") + 1;
            auto ifc_name = s.substr(start);
            if(ifc_name.back() == '\n')
                ifc_name.erase(ifc_name.size() - 1);
            active_interfaces_ip[ifc_name] = smtch[1];
        }
    }

    pclose(f);
    return active_interfaces_ip;
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
    return std::to_string(port.port);
}

std::string to_string(time_t t)
{
    auto loct{std::localtime(&t)};
    std::ostringstream buffer;
    buffer << std::put_time(loct, "%c");

    return buffer.str();
}
}
}
