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
#include <errno.h>
#include <unistd.h>
#include "TCPUtility.h"

#define MAXINCOME 5

// 只需要bind，不考虑本机的ip地址。
int main(int argc, char *argv[])
{
    // port

    if (argc < 2 || argc > 2)
        DieWithUserMessage("Parameters error"
                           "[<server Port>]");

    // creat serversocket
    in_port_t serverPort = atoi(argv[1]);
    if (serverPort == 0)
        DieWithUserMessage("atoi() failed");

    int serverSocket = setupTCPListen(argv[1]);
    if (serverSocket < 0)
        DieWithUserMessage("setupTCPListen failed");

    // 后面是一个无线循环的accept，然后处理请求的过程
    for (;;)
    {
        int clientSocket = acceptTCPConn(serverSocket);
        if (clientSocket < 0)
        {
            PRINTERROR("clientSocket");
        }
        HandleTCPClient(clientSocket);
    }

    // NOT reach
    close(serverSocket);

    return 0;
}