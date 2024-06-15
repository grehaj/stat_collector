#pragma once

#include "server/IServerRunGuard.h"

#include "gmock/gmock.h"

using namespace testing;

namespace server
{

class ServerRunGuardMock : public IServerRunGuard
{
public:
    MOCK_CONST_METHOD0(should_stop, bool());
};

}
