#include "System.h"

#include "Consts.h"
#include <regex>
#include <stdexcept>


namespace utils
{

std::map<std::string, std::string> System::get_active_interfaces_ip()
{
    FILE* f = popen("ip -4 addr", "r");
    if(f == nullptr)
        throw std::runtime_error{"SystemCommand: popen - ip addr"};

    std::regex r{R"(inet\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))"};
    std::smatch smtch;
    std::map<std::string, std::string> active_interfaces_ip;
    char buffer[READSIZE]{};

    while (fgets(buffer, READSIZE, f))
    {
        std::string s{buffer};
        if(std::regex_search(s, smtch, r))
        {
            auto start = s.find_last_of(" ") + 1;
            auto ifc_name = s.substr(start);
            if(ifc_name.back() == '\n')
                ifc_name.erase(ifc_name.size() - 1);
            active_interfaces_ip[ifc_name] = smtch[1];
        }
    }

    pclose(f);
    return active_interfaces_ip;
}

FILE* System::popen(const char *command, const char *type)
{
    return ::popen(command, type);
}

int System::pclose(FILE *stream)
{
    return ::pclose(stream);
}

}
