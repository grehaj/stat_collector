#pragma once

#include "Types.h"

#include <list>
#include <string>

namespace client
{
class TrafficStorage
{
public:
    using storage_t = std::list<utils::traffic_t>;
    using size_type = storage_t::size_type;

    TrafficStorage(size_type s);

    void update(const utils::traffic_t& data);
    size_type size() const;
    void clear();
    friend std::ostream& operator<<(std::ostream& out, const TrafficStorage& ts);

private:
    const size_type max_size;

    storage_t  traffic_storage;
};

}
