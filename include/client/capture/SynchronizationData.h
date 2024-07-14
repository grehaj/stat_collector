#pragma once

#include "TrafficStorage.h"
#include "Consts.h"
#include "TrafficStorage.h"

#include <condition_variable>
#include <mutex>


namespace client
{
namespace capture
{
struct SynchronizationData
{
    TrafficStorage traffic_storage{utils::storage_size};
    std::mutex storage_mutex;
    std::condition_variable produce;
    std::condition_variable consume;
};
}
}
