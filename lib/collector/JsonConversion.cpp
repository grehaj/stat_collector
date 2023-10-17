#include "JsonConversion.h"
#include "Utils.h"

namespace collector
{
void to_json(nlohmann::json& j, const utils::port_t& port)
{
    j = nlohmann::json{{"port", port.port}};
}

void to_json(nlohmann::json& j, const utils::ip_t& ip)
{
    j = nlohmann::json{{"ip", utils::ip_to_str(ip)}};
}

void to_json(nlohmann::json& j, const Socket& socket)
{
    j = nlohmann::json{{"ip", utils::ip_to_str(socket.ip)}, {"port", socket.port.port}};
}

void to_json(nlohmann::json& j, const counter_t& c)
{
    j = nlohmann::json{{"count", c.counter}};
}

void to_json(nlohmann::json& j, const PortTraffic& port_traffic)
{
    // RFC 8259 compilant json format
    j = nlohmann::json{};
    j["time"] = utils::to_string(port_traffic.observation_time);
    j["time_count"] = port_traffic.total_count.counter;
    j["ports"] = nlohmann::json::array();
    for(const auto& [p, data] : port_traffic.amount_per_dest_port)
    {
        nlohmann::json port;
        port["port"] = p.port;
        port["port_count"] = data.total_count.counter;
        port["details"] = nlohmann::json::array();
        for(const auto& [socket, count] : data.per_socket_count)
        {
            nlohmann::json detail;
            detail["src"] = socket;
            detail["src_count"] = count.counter;
            port["details"].push_back(detail);
        }
        j["ports"] .push_back(port);
    }
}
}
