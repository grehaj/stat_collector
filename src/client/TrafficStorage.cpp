#include "TrafficStorage.h"
#include "JsonConversion.h"
#include "TimeUtils.h"

#include <iomanip>
#include <numeric>

namespace client
{

using namespace utils;

TrafficStorage::TrafficStorage(size_type s) : max_size{s}
{
    if(max_size == 0)
    {
        throw std::runtime_error{"TrafficStorage::TrafficStorage storage_size=0"};
    }
}

void TrafficStorage::update(const utils::traffic_t& data)
{
    traffic_storage.push_back(data);

    if(traffic_storage.size() > max_size)
    {
        traffic_storage.pop_front();
    }
}

void TrafficStorage::clear()
{
    return traffic_storage.clear();
}

TrafficStorage::size_type TrafficStorage::size() const
{
    return traffic_storage.size();
}

std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts)
{
    nlohmann::json j{};
    j["stat"] = ts.traffic_storage;

    return out << std::setw(2) << j << std::endl;
}
}
