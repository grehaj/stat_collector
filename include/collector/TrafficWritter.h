#pragma once

#include "CollectorThread.h"
#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <mutex>
#include <regex>
#include <string>

namespace collector
{
class TrafficWritter : public CollectorThread
{
public:
    TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                   bool& finished, std::exception_ptr& err);

   void run(ThreadArg threadArg) override;
};
}
