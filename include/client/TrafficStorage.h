#pragma once

#include "TrafficData.h"
#include <cstdint>
#include <deque>

namespace collector
{
class TrafficStorage
{
public:
    using size_type = typename traffic_t::size_type;
    TrafficStorage(const std::string& ifc, const std::string& ip, size_type s);

    bool update(time_t t, const Connection& data);
    size_type size() const
    {
        return traffic.size();
    }
    void clear()
    {
        return traffic.clear();
    }

    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);
private:
    const std::string interface;
    const std::string ip_addr;
    const size_type max_secs;

    traffic_t traffic;
};

}
