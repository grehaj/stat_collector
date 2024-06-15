#pragma once

namespace server
{

class IServerRunGuard
{
public:
    virtual ~IServerRunGuard() = default;

    virtual bool should_stop() const = 0;
};

}
