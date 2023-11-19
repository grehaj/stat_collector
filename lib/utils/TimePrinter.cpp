#include "utils/TimePrinter.h"

#include <ctime>
#include <iomanip>

namespace utils
{
std::ostream& operator<<(std::ostream& os, const std::chrono::time_point<std::chrono::system_clock>& t)
{
    const auto time_t_rep{std::chrono::system_clock::to_time_t(t)};
    const auto local{std::localtime(&time_t_rep)};

    return os << std::put_time(local, "%c");
}
}
