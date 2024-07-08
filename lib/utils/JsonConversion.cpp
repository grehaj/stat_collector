#include "JsonConversion.h"
#include "TimeUtils.h"

#include <string>

namespace utils
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

void to_json(nlohmann::json& j, const utils::traffic_t& traffic)
{
    // RFC 8259 compilant json format
    j = nlohmann::json{};

    j["time"] = utils::to_string(traffic.t);
    j["src_ip"] = utils::ip_to_str(traffic.src_ip);
    j["src_port"] = utils::port_to_str(traffic.src_port);
    j["dst_ip"] = utils::ip_to_str(traffic.dst_ip);
    j["dst_port"] = utils::port_to_str(traffic.dst_port);
}
}
