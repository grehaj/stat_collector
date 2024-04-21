#pragma once

#include "TrafficStorage.h"

#include <string>

namespace client
{

class StatClient
{
public:
    StatClient(const std::string& interface, utils::storage_size_t storage_size, utils::file_count_t file_count);

    void run();

private:
    std::string get_interface_ip(const std::string& ifc) const;

    const std::string interface;
    const std::string ip;
    const utils::storage_size_t storage_size;
    const utils::file_count_t file_count;
    TrafficStorage traffic_storage;
    int writer_socket{-1};
};

}
