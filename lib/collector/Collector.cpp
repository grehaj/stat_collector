#include "Collector.h"
#include "TrafficReader.h"
#include "TrafficStorage.h"
#include "TrafficWritter.h"
#include "Utils.h"
#include <condition_variable>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <regex>
#include <thread>

namespace collector
{
namespace fs = std::filesystem;

Collector::Collector(const std::string& ifc, file_count_t fc, storage_size_t s, const std::string& d):
    interface{ifc}, ip{get_interface_ip(ifc)}, file_count{fc}, storage_size{s}, directory{d},
    traffic_storage{interface, ip, storage_size}
{
}

void Collector::run()
{
    std::mutex storage_mutex;
    std::condition_variable ready_to_write;
    bool finished = false;
    auto  threadArg = ThreadArg{file_count, storage_size, directory, interface, ip};
    std::exception_ptr exceptionPtr = nullptr;
    auto reader = TrafficReader{traffic_storage, storage_mutex, ready_to_write, finished, exceptionPtr};
    auto writter = TrafficWritter{traffic_storage, storage_mutex, ready_to_write, finished, exceptionPtr};
    std::thread reader_thread{reader, threadArg};
    std::thread writter_thread{writter, threadArg};
    reader_thread.join();
    writter_thread.join();

    if(exceptionPtr)
        std::rethrow_exception(exceptionPtr);
}

std::string Collector::get_interface_ip(const std::string& ifc) const
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw std::runtime_error{"Unable to find interface '" + ifc + "'."};

    return ifcs[ifc];
}
}
