#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include "errorPrint.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>

// #define BUFSIZE 500

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
        DieWithUserMessage("Parameters error "
                           "<Server ipv6 Address> <Echo Word> [<server Port>]");

    char *serverIP = argv[1];
    char *echoString = argv[2];

    in_port_t servePort = (argc == 4) ? atoi(argv[3]) : 18888;
    // 7 is a default echo port;

    int serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
        DieWithSystemMessage("socket() failed");
    fprintf(stdout, "stdout: create a socket for connecting: %d\n", serverSocket);

    struct sockaddr_in6 serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(servePort);

    int rtnVal = (inet_pton(AF_INET6, serverIP, &serverAddr.sin6_addr));
    if (rtnVal == 0)
        DieWithUserMessage("server IPv6 does not contain a vaild character address");
    else if (rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");

    // serverAddrLen =

    errno = 0;
    if ((connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
        DieWithSystemMessage("connect() failed");
    if (errno != 0)
    {
        perror("client connect server failed");
    }
    else
        fprintf(stdout, "client successfully connect to %s\n", serverIP);

        // client 应用层
    rtnVal = clientAppliction(serverSocket, echoString);
    if (rtnVal != 0)
        DieWithUserMessage("clientAppliction() error");
    return 0;
}
