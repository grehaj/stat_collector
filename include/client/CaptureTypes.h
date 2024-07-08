#pragma once

#include <set>
#include <string>

namespace client
{

struct hpcap_addr_t
{
    std::string network_address;
    std::string network_mask;

    bool operator<(const hpcap_addr_t& rhs) const
    {
        if(network_address < rhs.network_address)
            return true;
        else if(network_address > rhs.network_address)
            return false;

        return network_mask < rhs.network_mask;
    }
};

struct hpcap_if_t
{
    std::string device;
    std::set<hpcap_addr_t> addresses;
};
}
