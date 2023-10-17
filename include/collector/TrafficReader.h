#pragma once

#include "CollectorThread.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <fstream>
#include <memory>
#include <mutex>
#include <regex>

namespace collector
{
using namespace utils;

class TrafficReader : public CollectorThread
{
public:
    TrafficReader(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                  bool& finished, std::exception_ptr& err);

   void run(ThreadArg threadArg) override;

    inline static const std::regex r{
        R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
};

}
