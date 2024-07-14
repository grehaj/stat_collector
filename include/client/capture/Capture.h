#pragma once

#include "CaptureTypes.h"

#include <string_view>

#include <pcap/pcap.h>

namespace client
{
class Capture
{
public:
	Capture(std::string_view device);
	Capture(const Capture&) = delete;
	const Capture& operator=(const Capture&) = delete;
	Capture(Capture&&) = delete;
	const Capture& operator=(Capture&&) = delete;
	~Capture();

	void run(u_char * arg) const;

	static void packet_handler(u_char *args, const pcap_pkthdr *header, const u_char *packet);

private:
	hpcap_addr_t pcap_addr_to_h(const pcap_addr_t* addr);
	hpcap_if_t get_pcap_if(std::string_view dev);
	std::string af_inte_to_h(const in_addr& add);

	pcap_t * handle{};
};
}
