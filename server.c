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

#define MAXINCOME 5

// 只需要bind，不考虑本机的ip地址。
int main(int argc, char *argv[])
{
    // port

    if (argc < 2 || argc > 2)
        DieWithUserMessage("Parameters error"
                           "[<server Port>]");

    // creat serversocket
    in_port_t serverPort = atoi(argv[2]);
    if (serverPort == 0)
        DieWithUserMessage("atoi() failed");

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // new a sockaddr for server binding

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    // bind

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
        DieWithSystemMessage("bind() failed");

    // fprintf可以用控制输出的格式控制符
    fprintf(stdout, "Server Bind to %lu port successfully", serverPort);

    // listen and get a new socket

    int listenSocket = listen(serverSocket, MAXINCOME);
    if (listenSocket == -1)
        DieWithSystemMessage("listen() failed");

    // listen
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizof(clientAddr));
    ssize_t clientAddrLen = 

    if((accept(listenSocket, (struct sockaddr *)&clientAddr, )
}