#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <condition_variable>
#include <exception>
#include <mutex>

namespace collector
{
struct ThreadArg
{
    utils::file_count_t file_count;
    utils::storage_size_t storage_size;
    std::string directory;
    std::string interface_name;
    std::string interface_ip;
};

class CollectorThread
{
public:
    CollectorThread(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                    bool& finished, std::exception_ptr& err);
    virtual ~CollectorThread() = default;
    void operator()(ThreadArg threadArg);

protected:
    virtual void run(ThreadArg threadArg) = 0;

    TrafficStorage& traffic_storage;
    std::mutex& storage_mtx;
    std::condition_variable& ready_to_write;
    bool& finished;
    std::exception_ptr& error;
};
}
