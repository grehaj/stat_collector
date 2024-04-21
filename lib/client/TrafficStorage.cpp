#include "TrafficStorage.h"
#include "JsonConversion.h"
#include "TimeUtils.h"

#include <iomanip>
#include <numeric>

namespace client
{

using namespace utils;

TrafficStorage::TrafficStorage(const std::string& ifc, const std::string& ip, size_type s) :
    interface{ifc}, ip_addr{ip}, max_secs{s}
{
    if(s == 0)
    {
        throw std::runtime_error{"TrafficStorage::TrafficStorage storage_size=0"};
    }
}

void TrafficStorage::update(const TrafficData& data)
{
    traffic.push_back(data);

    if(traffic.size() > max_secs)
    {
        traffic.pop_front();
    }
}

void TrafficStorage::clear()
{
    return traffic.clear();
}

TrafficStorage::size_type TrafficStorage::size() const
{
    return traffic.size();
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    nlohmann::json j{};
    j["stat"] = ts.traffic;

    return out << std::setw(2) << j << std::endl;
}
}
