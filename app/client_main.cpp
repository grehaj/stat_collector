#include "StatClient.h"
#include "Socket.h"
#include "System.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argv, char* argc[]) try
{
    client::StatClient client{std::make_unique<utils::Socket>(), std::make_unique<utils::System>(), "wlp2s0", 1};
    client.run();
    std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
}
