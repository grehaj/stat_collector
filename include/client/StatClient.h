#pragma once

#include "ISocket.h"
#include "ISystem.h"
#include "TrafficStorage.h"

#include <memory>
#include <string>

namespace client
{

class StatClient
{
public:
    StatClient(std::unique_ptr<utils::ISocket> soc, std::unique_ptr<utils::ISystem> sys, const std::string& interface, utils::storage_size_t storage_size);

    void run();

private:
    std::string get_interface_ip(const std::string& ifc) const;

    std::unique_ptr<utils::ISocket> soc;
    std::unique_ptr<utils::ISystem> system;
    const std::string interface;
    const std::string ip;
    const utils::storage_size_t storage_size;
    TrafficStorage traffic_storage;
    int writer_socket{-1};
};

}
