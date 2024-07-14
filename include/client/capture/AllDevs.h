#pragma once

#include <pcap/pcap.h>

namespace client
{
namespace capture
{
class AllDevs
{
public:
	AllDevs();
	AllDevs(const AllDevs&) = delete;
	AllDevs& operator=(const AllDevs&) = delete;
	AllDevs(AllDevs&&) = delete;
	AllDevs& operator=(AllDevs&&) = delete;

	class iterator
	{
	public:
		explicit iterator(pcap_if_t *d);
		iterator& operator++();
		const pcap_if_t& operator*();
		bool operator!=(const iterator& rhs) const;
		bool operator==(const iterator& rhs) const;
	private:
		pcap_if_t *dev{};
	};
	iterator begin();
	iterator end();


	~AllDevs();

private:
	pcap_if_t *alldevsp{};
};
}
}
