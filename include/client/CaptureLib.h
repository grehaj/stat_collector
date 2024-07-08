#pragma once

#include "CaptureTypes.h"
#include "Consts.h"
#include "TrafficStorage.h"

#include <pcap/pcap.h>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <string_view>

namespace client
{
struct SynchronizationData
{
    TrafficStorage traffic_storage{utils::storage_size};
    std::mutex storage_mutex;
    std::condition_variable produce;
    std::condition_variable consume;
};

pcap_t* open_live(std::string_view device);
hpcap_addr_t pcap_addr_to_h(const pcap_addr_t* addr);
hpcap_if_t get_pcap_if(std::string_view dev);
std::string af_inte_to_h(const in_addr& add);
void packet_handler(u_char *args, const pcap_pkthdr *header, const u_char *packet);
// TODO close function
}
