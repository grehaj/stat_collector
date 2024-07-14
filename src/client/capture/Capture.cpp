#include "Capture.h"

#include "capture/AllDevs.h"
#include "capture/SynchronizationData.h"

#include <stdexcept>

#include <netinet/in.h>
#include <netinet/if_ether.h>

namespace client
{

Capture::Capture(std::string_view device)
{
    const int packet_count_limit = 1;
    const int timeout_limit = 10000;
    char error_buffer[PCAP_ERRBUF_SIZE];
    const auto ifc = get_pcap_if(device);

    handle = pcap_open_live(ifc.device.c_str(), BUFSIZ, packet_count_limit, timeout_limit, error_buffer);

    for(const auto& add : ifc.addresses)
    {
    	const std::string filter_exp = std::string{"dst "} + add.network_address;
    	bpf_program filter;
    	if(pcap_compile(handle, &filter, filter_exp.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1)
    	{
    		throw std::runtime_error{filter_exp + " cannot be compiled"};
    	}
    	if(pcap_setfilter(handle, &filter))
        {
        	throw std::runtime_error{filter_exp + " cannot be set"};
        }
    }

    if(handle == nullptr)
    {
        throw std::runtime_error{error_buffer};
    }
}

Capture::~Capture()
{
	pcap_close(handle);
}

void Capture::run(u_char * arg) const
{
    pcap_loop(handle, 0, packet_handler, arg);
}

hpcap_addr_t Capture::pcap_addr_to_h(const pcap_addr_t* addr)
{
    hpcap_addr_t result{};
    for(auto a = addr; a != NULL; a = a->next)
    {
        const auto address = a->addr;
        switch(address->sa_family)
        {
            case AF_INET:
                result.network_address = af_inte_to_h(reinterpret_cast<sockaddr_in*>(a->addr)->sin_addr);
                result.network_mask = af_inte_to_h(reinterpret_cast<sockaddr_in*>(a->netmask)->sin_addr);
                break;
            default:
                break;
        }
    }
    return result;
}


hpcap_if_t Capture::get_pcap_if(std::string_view dev)
{
	capture::AllDevs allDevs;
	hpcap_if_t result{};
	for(auto d : allDevs)
	{
		if(d.name == dev)
		{
			result.device = d.name;
			for(auto a = d.addresses; a != nullptr; a = a->next)
			{
				if(const auto pa = pcap_addr_to_h(a);  not pa.network_address.empty())
				{
					result.addresses.insert(pa);
				}
			}
			return result;
		}
	}
    throw std::runtime_error{std::string{dev} + " not found"};
}

std::string Capture::af_inte_to_h(const in_addr& add)
{
    char ip[INET_ADDRSTRLEN]{};
    if (inet_ntop(AF_INET, &add.s_addr, ip, INET_ADDRSTRLEN) == nullptr)
    {
        throw std::runtime_error{"inet_ntoa"};
    }

    return ip;
}

void Capture::packet_handler(u_char *args, const pcap_pkthdr *header, const u_char *packet)
{
	capture::SynchronizationData* sd = reinterpret_cast<capture::SynchronizationData*>(args);
	TrafficStorage& ts = sd->traffic_storage;
	std::mutex& mtx = sd->storage_mutex;
	std::condition_variable& go_produce = sd->produce;
	std::condition_variable& go_consume = sd->consume;

	ether_header *eth_header = reinterpret_cast<ether_header*>(const_cast<u_char *>(packet));
	if (ntohs(eth_header->ether_type) != ETHERTYPE_IP)
	{
		return;
	}

	const int ethernet_header_length = 14;

	const u_char *ip_header = packet + ethernet_header_length;
	int ip_header_length = ((*ip_header) & 0x0F) * 4;

	u_char protocol = *(ip_header + 9);
	if (protocol != IPPROTO_TCP and protocol != IPPROTO_UDP)
	{
		return;
	}

	const uint32_t* src_ip = reinterpret_cast<const uint32_t*>(ip_header) + 3;
	const uint32_t* dst_ip = reinterpret_cast<const uint32_t*>(ip_header) + 4;

	const u_char *tcp_header = packet + ethernet_header_length + ip_header_length;
	const uint16_t* src_port = reinterpret_cast<const uint16_t*>(tcp_header);
	const uint16_t* dst_port = reinterpret_cast<const uint16_t*>(tcp_header) + 1;

	std::unique_lock<std::mutex> ul{mtx};
	go_produce.wait(ul, [&]() {return ts.size() < utils::storage_size;});
	auto t = utils::traffic_t{std::time(nullptr), utils::ip_t{*src_ip}, utils::port_t{*src_port}, utils::ip_t{*dst_ip}, utils::port_t{*dst_port}};
    ts.update(t);

    if(ts.size() == utils::storage_size)
    {
    	go_consume.notify_one();
    }
}
}
