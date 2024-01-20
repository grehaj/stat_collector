#include "Collector.h"

#include <cstdlib>
#include <iostream>

using namespace collector;

using namespace std;

int main(int argv, char* argc[]) try
{
    collector::Collector{"wlp2s0", 3, 10, "whatever"}.run();
    std::exit(EXIT_SUCCESS);
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    std::exit(EXIT_FAILURE);
}
