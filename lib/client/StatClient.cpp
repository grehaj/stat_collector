#include "StatClient.h"
#include "Collector.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace network
{

StatClient::StatClient()
{
}

void StatClient::run()
{
    collector::Collector{"wlp2s0", 1, 1}.run();
}

}
