#include "JsonConversion.h"
#include "TimeUtils.h"

#include <string>

namespace client
{
void to_json(nlohmann::json& j, const utils::port_t& port)
{
    j = nlohmann::json{{"port", port.port}};
}

void to_json(nlohmann::json& j, const utils::ip_t& ip)
{
    j = nlohmann::json{{"ip", ip_to_str(ip)}};
}

void to_json(nlohmann::json& j, const utils::counter_t& c)
{
    j = nlohmann::json{{"count", c.counter}};
}

void to_json(nlohmann::json& j, const TrafficData& trafficData)
{
    // RFC 8259 compilant json format
    j = nlohmann::json{};

    j["time"] = utils::to_string(trafficData.t);
    j["src_ip"] = utils::ip_to_str(trafficData.src_ip);
    j["src_port"] = utils::port_to_str(trafficData.src_port);
    j["dst_ip"] = utils::ip_to_str(trafficData.dst_ip);
    j["dst_port"] = utils::port_to_str(trafficData.dst_port);
}
}
