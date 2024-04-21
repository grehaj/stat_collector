#pragma once

#include "TrafficData.h"

#include <list>

namespace client
{
class TrafficStorage
{
public:
    using traffic_t = std::list<TrafficData>;
    using size_type = traffic_t::size_type;

    TrafficStorage(const std::string& ifc, const std::string& ip, size_type s);

    void update(const TrafficData& data);
    size_type size() const;
    void clear();
    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);

private:
    const std::string interface;
    const std::string ip_addr;
    const size_type max_secs;

    std::list<TrafficData> traffic;
};

}
