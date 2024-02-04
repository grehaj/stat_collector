#include "TrafficWritter.h"
#include "utils/SocketUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace client
{

using namespace utils;

TrafficWritter::TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                               bool& finished, std::exception_ptr& error):
    CollectorThread{ts, m, cv, finished, error}, writer_socket{socket(AF_UNIX, SOCK_DGRAM, 0)}
{
    if(writer_socket == -1)
    {
        throw std::runtime_error{"writer - socket"};
    }
}

void TrafficWritter::run(ThreadArg threadArg)
{
    sockaddr_un add{get_socket_address()};
    file_count_t file_num = 1;

    while(true)
    {
        std::unique_lock<std::mutex> ul{storage_mtx};
        ready_to_write.wait(ul, [&]{return traffic_storage.size() == threadArg.storage_size;});
        std::stringstream out{};
        out << traffic_storage;
        std::string line;
        while(std::getline(out, line))
        {
            auto msg_size = line.length();
            if(msg_size > READSIZE)
            {
                line = "error";
                msg_size = line.length();
            }
            if(sendto(writer_socket, line.c_str(), msg_size, 0, reinterpret_cast<sockaddr*>(&add), sizeof (sockaddr_un)) != msg_size)
            {
                throw std::runtime_error{"writer - sendto"};
            }
        }

        traffic_storage.clear();
        ++file_num;
        if(file_num > threadArg.file_count)
        {
            finished = true;
            return;
        }
    }
}
}
