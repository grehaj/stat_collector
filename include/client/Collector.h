#pragma once

#include "TrafficStorage.h"
#include "Utils.h"
#include <memory>
#include <string>

namespace client
{
class Collector
{
public:
    Collector(const std::string& interface,
              utils::file_count_t file_count,
              utils::storage_size_t storage_size);

    void run();

private:
    std::string get_interface_ip(const std::string& ifc) const;

    const std::string interface;
    const std::string ip;
    const utils::file_count_t file_count;
    const utils::storage_size_t storage_size;
    const std::string directory;

    TrafficStorage traffic_storage;
};
}
