#pragma once

#include "IServerRunGuard.h"

namespace server
{

class ServerRunGuard : public IServerRunGuard
{
public:
    bool should_stop() const override;
};

}
