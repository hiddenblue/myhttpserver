#include "TCPUtility.h"

int setupTCPServerconn(const char *servername, const char *port)
{
    if (servername == NULL || port == NULL)
        DieWithUserMessage("servername or port NULL pointer error");

    struct addrinfo addrInfoHint;
    memset(&addrInfoHint, 0, sizeof(addrInfoHint));
    addrInfoHint.ai_family = AF_UNSPEC;
    addrInfoHint.ai_protocol = IPPROTO_TCP;
    addrInfoHint.ai_socktype = SOCK_STREAM;

    struct addrinfo *addrList;

    int rtnVal = getaddrinfo(servername, port, &addrInfoHint, &addrList);
    if (rtnVal != 0)
    {
        DieWithSystemMessage("getaddrinfo() failed");
        fputs(gai_strerror(rtnVal), stderr);
        fputc("\n", stderr);
    }

    int serverSocket = -1;
    for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next)
    {
        PrintSockaddr(addr->ai_addr, stdout);

        int serverSocket = socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket < 0)
        {
            fprintf("create socket for server :%s %s error\n", servername, port);
            continue;
        }
        fprintf("successfully create socket for server %s \n", servername);

        errno = 0;
        if ((connect(serverSocket, (struct sockaddr *)addr->ai_addr, addr->ai_addrlen)) != 0)
        {

            if (errno != 0)
            {
                perror("client connect server failed");
                continue;
            }
            else
                fprintf(stdout, "client successfully connect to %s\n", servername);
        }
    }
}