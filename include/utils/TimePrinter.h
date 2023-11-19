#pragma once

#include <chrono>
#include <ostream>

namespace utils
{
std::ostream& operator<<(std::ostream& os, const std::chrono::time_point<std::chrono::system_clock>& t);
}
