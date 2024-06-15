#pragma once

#include <map>
#include <stdio.h>
#include <string>

namespace utils
{

class ISystem
{
public:
    virtual ~ISystem() = default;

    virtual std::map<std::string, std::string> get_active_interfaces_ip() = 0;
    virtual FILE *popen(const char *command, const char *type) = 0;
    virtual int pclose(FILE *stream) = 0;
};

}
