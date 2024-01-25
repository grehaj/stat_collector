#pragma once

namespace network
{

class ISocket
{
public:
    virtual ~ISocket() = default;
    virtual void run() = 0;
};

}
