#include <iostream>
#include <stdexcept>

#include <csignal>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

constexpr char* PORT_NUM = "50000";
constexpr int BACKLOG = 50;

using namespace std;

int main(int argv, char* argc[])
{
    int seq_num = 0;
    std::signal(SIGPIPE, SIG_IGN);
    addrinfo hints{};
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;


    addrinfo *result;
    if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0)
        throw std::runtime_error{"getaddrinfo"};

    addrinfo *rp;
    int optval = -1;
    int lfd = 0;
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(lfd == -1)
        {
            continue;
        }
        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            throw std::runtime_error("setsockopt");

        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(lfd);
    }

    if (rp == NULL)
        throw std::runtime_error{"Could not bind socket to any address"};

    if (listen(lfd, BACKLOG) == -1)
        throw std::runtime_error{"listen"};

    freeaddrinfo(result);

    unsigned addrlen = -1;
    int cfd = -1;
    sockaddr_storage claddr;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    #define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    while(true)
    {
        addrlen = sizeof(sockaddr_storage);
        cfd = accept(lfd, (sockaddr *) &claddr, &addrlen);
        if (cfd == -1)
        {
            cerr <<  "accept" << endl;
            continue;
        }


        if (getnameinfo((struct sockaddr *) &claddr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
            snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
        else
            snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
        cout << "Connection from " << addrStr << endl;

        //TODO read data wform socket cfd
        // write response to client
    }


    return 0;
}
