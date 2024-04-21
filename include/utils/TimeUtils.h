#pragma once

#include <ctime>
#include <string>

namespace utils
{
time_t get_time();
std::string get_ctime();
std::string to_string(time_t t);
}
