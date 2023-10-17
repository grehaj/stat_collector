#include "TrafficReader.h"

namespace collector
{
using namespace utils;

TrafficReader::TrafficReader(TrafficStorage& ts, std::mutex& m,
                             std::condition_variable& cv, bool& finished, std::exception_ptr& error):
    CollectorThread{ts, m, cv, finished, error}
{
}

void TrafficReader::run(ThreadArg threadArg)
{
    const std::string tcp_dump_command = std::string{
            "tcpdump -n -tt -i "} + threadArg.interface_name + " dst " + threadArg.interface_ip;
    using del_t = decltype (fifo_deleter<FILE>());
    auto f = std::unique_ptr<FILE, del_t>(popen(tcp_dump_command.c_str(), "r"), utils::fifo_deleter<FILE>());
    if(f == nullptr)
        throw std::runtime_error{"SystemCommand: popen - tcpdump"};
    std::smatch sm;
    char buffer[utils::READSIZE]{};
    while (not finished and fgets(buffer, utils::READSIZE, f.get()))
    {
        std::string s{buffer};
        if(std::regex_search(s, sm, r))
        {
            auto d = Connection{{str_to_ip(sm[1]), str_to_port(sm[2])}, {str_to_ip(sm[3]), str_to_port(sm[4])}};
            auto end_pos = s.find(".");
            time_t tmime_stamp = std::stoull(s.substr(0, end_pos));
            {
                std::lock_guard<std::mutex> lg{storage_mtx};
                traffic_storage.update(tmime_stamp, d);
            }
            if(traffic_storage.size() == threadArg.storage_size)
                ready_to_write.notify_one();
        }
    }
}
}
