#include "AllDevs.h"

#include <stdexcept>

namespace client
{
namespace capture
{

AllDevs::AllDevs()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	const auto pcap_findalldevs_output = pcap_findalldevs(&alldevsp, errbuf);
	if(pcap_findalldevs_output == PCAP_ERROR)
	    throw std::runtime_error{errbuf};
}

AllDevs::iterator::iterator(pcap_if_t *d): dev{d}
{
}

AllDevs::iterator& AllDevs::iterator::operator++()
{
	dev = dev->next;
	return *this;
}

const pcap_if_t& AllDevs::iterator::operator*()
{
	return *dev;
}

bool AllDevs::iterator::operator!=(const iterator& rhs) const
{
	return dev != rhs.dev;
}

bool AllDevs::iterator::operator==(const iterator& rhs) const
{
	return dev == rhs.dev;
}

AllDevs::iterator AllDevs::begin() {return iterator{alldevsp};};
AllDevs::iterator AllDevs::end() {return iterator{nullptr};};


AllDevs::~AllDevs()
{
	pcap_freealldevs(alldevsp);
}
}
}
