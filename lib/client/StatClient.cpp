#include "StatClient.h"
#include "Consts.h"
#include "Utils.h"
#include <regex>
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

namespace client
{

StatClient::StatClient(const std::string& ifc, utils::storage_size_t storage_size, utils::file_count_t file_count):
    interface{ifc}, ip{get_interface_ip(ifc)}, storage_size{storage_size}, file_count{file_count}, traffic_storage{interface, ip, storage_size}
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

    if (getaddrinfo("localhost", utils::PORT_NUM.c_str(), &hints, &result) != 0)
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

void StatClient::run()
{
    const std::string tcp_dump_command = std::string{"tcpdump -n -tt -i "} + "wlp2s0" + " dst " + ip;
    using del_t = decltype (utils::fifo_deleter<FILE>());
    auto f = std::unique_ptr<FILE, del_t>(popen(tcp_dump_command.c_str(), "r"), del_t{});
    if(f == nullptr)
        throw std::runtime_error{"SystemCommand: popen - tcpdump"};

    const std::regex r{R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+)\s>\s+(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})\.(\d+))"};
    std::smatch sm;
    char buffer[utils::READSIZE]{};
    utils::file_count_t file_num = 1;
    while (file_num <= file_count and fgets(buffer, utils::READSIZE, f.get()))
    {
        std::string s{buffer};
        if(std::regex_search(s, sm, r))
        {
            auto end_pos = s.find(".");
            time_t ts = std::stoull(s.substr(0, end_pos));
            auto d = TrafficData{ts, utils::str_to_ip(sm[1]), utils::str_to_port(sm[2]), utils::str_to_ip(sm[3]), utils::str_to_port(sm[4])};
            traffic_storage.update(d);

            if(traffic_storage.size() == storage_size)
            {
                std::stringstream out{};
                out << traffic_storage;
                std::string line;
                while(std::getline(out, line))
                {
                    auto msg_size = line.length();
                    if(msg_size > utils::READSIZE)
                    {
                        line = "error";
                        msg_size = line.length();
                    }
                    if(write(writer_socket, line.c_str(), msg_size) != msg_size)
                    {
                        throw std::runtime_error{std::string("writer - write data: ") + strerror(errno)};
                    }
                    if (write(writer_socket, "\n", 1) != 1)
                    {
                        throw std::runtime_error{std::string("writer - write new line: ") + strerror(errno)};
                    }
                }
                traffic_storage.clear();
                ++file_num;
            }
        }
    }
}

std::string StatClient::get_interface_ip(const std::string& ifc) const
{
    auto ifcs{utils::get_active_interfaces_ip()};
    if(ifcs.find(ifc) == ifcs.end())
        throw std::runtime_error{"Unable to find interface '" + ifc + "'."};

    return ifcs[ifc];
}

}
