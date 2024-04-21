#include "SocketUtils.h"
#include "Utils.h"

#include <stdexcept>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace utils
{

std::string readLine(int fd)
{
    ssize_t numRead{};
    std::string result;
    char ch{};

    while( true )
    {
        numRead = read(fd, &ch, 1);
        if (numRead == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                throw std::runtime_error{std::string{"readLine:" } + strerror(errno)};
            }
        } else if (numRead == 0)
        {
            break;
        }
        else
        {
            result += ch;
            if (ch == '\n')
            {
                break;
            }
        }
    }

    return result;
}
}
