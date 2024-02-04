#include "StatClient.h"
#include "Collector.h"

#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

namespace client
{

StatClient::StatClient()
{
}

void StatClient::run()
{
    Collector{"wlp2s0", 1, 1}.run();
}

}
