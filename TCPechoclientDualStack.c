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
#include <netinet/in.h>
#include <TCPUtility.h>

// #define BUFSIZE 500

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
        DieWithUserMessage("Parameters error "
                           "<Server Name> <Echo Word> [<server Port>]");

    char *serverName = argv[1];
    char *echoString = argv[2];

    char *serverPort = (argc == 4) ? argv[4] : "7";
    // in_port_t servePort = (argc == 4) ? atoi(argv[3]) : 7;
    // 7 is a default echo port;

    int serverSocket = setupTCPServerconn(serverName, serverPort);

    // print peername and remote name

    printRemoteSocket(AF_INET, serverSocket);
    printLocalSocket(AF_INET, serverSocket);

    // client 应用层
    int rtnVal = clientAppliction(serverSocket, echoString);
    if (rtnVal != 0)
        DieWithUserMessage("clientAppliction() error");
    return 0;
}
