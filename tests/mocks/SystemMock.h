#pragma once

#include "ISystem.h"

#include "gmock/gmock.h"

using namespace testing;

namespace utils
{

class SystemtMock : public ISystem
{
public:
    MOCK_METHOD0(get_active_interfaces_ip, std::map<std::string, std::string>());
    MOCK_METHOD2(popen, FILE*(const char *command, const char *type));
    MOCK_METHOD1(pclose, int(FILE *stream));
};


}
