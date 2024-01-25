#include "StatClient.h"

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argv, char* argc[]) try
{
    network::StatClient client{};
    client.run();
    std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
}
