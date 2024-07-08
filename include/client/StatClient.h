#pragma once

#include "ISocket.h"
#include "ISystem.h"
#include "Types.h"

#include <memory>
#include <string.h>

namespace client
{
class StatClient
{
public:
    StatClient(std::unique_ptr<utils::ISocket> soc, std::unique_ptr<utils::ISystem> sys, const std::string& dev);

    void run();

private:
    std::unique_ptr<utils::ISocket> soc;
    std::unique_ptr<utils::ISystem> system;
    const std::string device;
    int writer_socket{-1};
};
}
