#include "JsonConversion.h"
#include "TrafficStorage.h"
#include <iomanip>
#include <numeric>

namespace collector
{
TrafficStorage::TrafficStorage(const std::string& ifc, const std::string& ip, size_type s) :
    interface{ifc}, ip_addr{ip}, max_secs{s}
{
}

bool TrafficStorage::update(time_t observation_time, const Connection& data)
{
    bool is_new_second{false};
    if(traffic.empty())
    {
        traffic.push_back(PortTraffic{observation_time});
    }

    const auto old_back = traffic.back();
    if(old_back.observation_time < observation_time)
    {
        for(auto t = old_back.observation_time + 1; t <= observation_time; ++t)
        {
            traffic.push_back(PortTraffic{t});
            is_new_second = true;
        }
    }

    auto& new_back = traffic.back();
    new_back += data;

    while(traffic.size() > max_secs)
    {
        traffic.pop_front();
    }

    return is_new_second;
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    if(ts.traffic.empty())
        return out;

    nlohmann::json j{};
    uint64_t init{0};
    j["interface_ip"] = ts.ip_addr;
    j["interface_name"] = ts.interface;
    j["stat"] = ts.traffic;
    j["stat_count"] = std::accumulate(ts.traffic.begin(), ts.traffic.end(), init,
                                       [](uint64_t c, const auto& d) {return c + d.total_count.counter;});
    j["period_start"] = utils::to_string(ts.traffic.front().observation_time);
    j["period_length"] = ts.max_secs;

    return out << std::setw(2) << j << std::endl;
}
}
