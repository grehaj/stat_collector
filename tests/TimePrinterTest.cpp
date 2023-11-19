#include "utils/TimePrinter.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "gmock/gmock.h"

using namespace utils;
using namespace testing;

class CoutWrapper
{
public:
    CoutWrapper()
    {
        std::cout.rdbuf(os.rdbuf());
    }
    std::string get_data() const
    {
        return os.str();
    }

    ~CoutWrapper()
    {
        std::cout.rdbuf(oldCoutStreamBuf);
    }

private:
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::stringstream os;

};

TEST(TimePrintTest, given_timepoint_output_operator_prints_epoch)
{
    time_t t0 = 0;
    const auto tp0 = std::chrono::system_clock::from_time_t(t0);
    CoutWrapper cw;
    std::cout <<  tp0;
    ASSERT_THAT(cw.get_data(), Eq("Thu Jan  1 01:00:00 1970"));
}

TEST(TimePrintTest, given_timepoint_output_operator_prints_hour_after_epoch)
{
    time_t t0 = 60;
    const auto tp0 = std::chrono::system_clock::from_time_t(t0);
    CoutWrapper cw;
    std::cout <<  tp0;
    ASSERT_THAT(cw.get_data(), Eq("Thu Jan  1 01:01:00 1970"));
}
