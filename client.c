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

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
        DieWithUserMessage("Parameters error"
                           "<Server Address> <Echo Word> [<server Port>]");

    char *serverIP = argv[1];
    char *echoString = argv[2];

    in_port_t servePort = (argc == 4) ? atoi(argv[3]) : 7;
    // 7 is a default echo port;

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(servePort);

    int rtnVal;
    if ((rtnVal = (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr.s_addr))) != 1)
        DieWithSystemMessage("inet_pton() failed");

    // serverAddrLen =

    if ((connect(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr))) != 0)
        DieWithSystemMessage("connect() failed");
    fputs("debug\n", stdout);

    return 0;
}