#include "utils/TimeUtils.h"

#include <stdexcept>

namespace utils
{
/*
Returns number of seconds since epoch
UTC (previously GMT)*/
time_t get_time()
{
    return  std::time(nullptr);
}

std::string get_ctime()
{
    const auto t = get_time();
    const auto c_time = ctime(&t);
    if(c_time == nullptr)
        throw std::runtime_error{"get_ctime - ctime returned nullptr"};

    return std::string{c_time};
}
}
