#pragma once

#include "TrafficData.h"
#include "Utils.h"
#include <nlohmann/json.hpp>

namespace collector
{
void to_json(nlohmann::json& j, const utils::port_t& port);
void to_json(nlohmann::json& j, const utils::ip_t& ip);
void to_json(nlohmann::json& j, const Socket& socket);
void to_json(nlohmann::json& j, const counter_t& c);
void to_json(nlohmann::json& j, const PortTraffic& pt);
}
