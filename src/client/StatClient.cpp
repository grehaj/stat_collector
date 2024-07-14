#include "StatClient.h"
#include "Consts.h"
#include "Capture.h"
#include "SynchronizationData.h"
#include "TrafficStorage.h"
#include "Utils.h"

#include <iostream>
#include <sstream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

namespace client
{

StatClient::StatClient(std::unique_ptr<utils::ISocket> socInit, std::unique_ptr<utils::ISystem> sys, const std::string& dev):
    soc{std::move(socInit)}, system{std::move(sys)}, device{dev}
{
    addrinfo hints{};
    addrinfo *result{};
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (soc->getaddrinfo("localhost", utils::PORT_NUM.c_str(), &hints, &result) != 0)
    {
        throw std::runtime_error{"client - getaddinfo"};
    }

    auto rp = result;
    for (; rp; rp = rp->ai_next)
    {
        writer_socket = soc->socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (writer_socket == -1)
            continue;

        if (soc->connect(writer_socket, rp->ai_addr, rp->ai_addrlen) != -1)
            break;

        soc->close(writer_socket);
    }

    if (rp == NULL)
    {
        throw std::runtime_error{"client - Could not connect socket to any address"};
    }

    soc->freeaddrinfo(result);
}

void StatClient::run()
{
    capture::SynchronizationData sd{};
    auto traffic_producer =  [&]()
    {
        client::Capture c{device};
        c.run(reinterpret_cast<u_char *>(&sd));
    };

    auto traffic_consumer =  [&]()
    {
    	TrafficStorage& ts = sd.traffic_storage;
    	std::mutex& mtx = sd.storage_mutex;
    	std::condition_variable& go_produce = sd.produce;
    	std::condition_variable& go_consume = sd.consume;
        while(true)
        {
            std::unique_lock<std::mutex> ul{mtx};
            go_consume.wait(ul, [&]{return ts.size() == utils::storage_size;});
            std::stringstream out{};
            out << ts;
            std::string line;
            while(std::getline(out, line))
            {
                ssize_t msg_size = line.length();
                if(msg_size > utils::READSIZE)
                {
                    line = "error";
                    msg_size = line.length();
                }
                if(soc->write(writer_socket, const_cast<char*>(line.c_str()), msg_size) != msg_size)
                {
                    throw std::runtime_error{std::string("writer - write data: ") + strerror(errno)};
                }
                if (soc->write(writer_socket, const_cast<char*>("\n"), 1) != 1)
                {
                    throw std::runtime_error{std::string("writer - write new line: ") + strerror(errno)};
                }
            }
            ts.clear();
            go_produce.notify_one();
        }
    };

    std::thread producer_thread{traffic_producer};
    std::thread consumer_thread{traffic_consumer};
    producer_thread.join();
    consumer_thread.join();
    //TODO flush the buffer
}

}
