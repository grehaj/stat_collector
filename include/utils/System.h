#pragma once

#include "ISystem.h"

namespace utils
{

class System : public ISystem
{
public:
    std::map<std::string, std::string> get_active_interfaces_ip() override;
    FILE *popen(const char *command, const char *type) override;
    int pclose(FILE *stream) override;
};

}
