#pragma once

#include "TrafficData.h"
#include "Utils.h"

#include <nlohmann/json.hpp>

namespace client
{
void to_json(nlohmann::json& j, const utils::port_t& port);
void to_json(nlohmann::json& j, const utils::ip_t& ip);
void to_json(nlohmann::json& j, const utils::counter_t& c);
void to_json(nlohmann::json& j, const TrafficData& trafficData);
}
