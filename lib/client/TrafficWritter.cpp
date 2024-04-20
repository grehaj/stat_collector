#include "TrafficWritter.h"
#include "utils/SocketUtils.h"

#include <iostream>
#include <string>
#include <sstream>

#include <netdb.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace client
{

using namespace utils;

TrafficWritter::TrafficWritter(TrafficStorage& ts, std::mutex& m, std::condition_variable& cv,
                               bool& finished, std::exception_ptr& error):
    CollectorThread{ts, m, cv, finished, error}
{
    addrinfo hints{};
    addrinfo *result{};
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo("localhost", PORT_NUM.c_str(), &hints, &result) != 0)
    {
        throw std::runtime_error{"client - getaddinfo"};
    }

    auto rp = result;
    for (; rp; rp = rp->ai_next)
    {
        writer_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (writer_socket == -1)
            continue;

        if (connect(writer_socket, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        close(writer_socket);
    }

    if (rp == NULL)
    {
        throw std::runtime_error{"client - Could not connect socket to any address"};
    }
}

void TrafficWritter::run(ThreadArg threadArg)
{
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
            std::cout << line << std::endl;
            if(msg_size > READSIZE)
            {
                line = "error";
                msg_size = line.length();
            }

            if(write(writer_socket, line.c_str(), msg_size) != msg_size)
            {
                throw std::runtime_error{"writer - write"};
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
