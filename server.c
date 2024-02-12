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

    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // new a sockaddr for server binding
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    // bind
    errno = 0;
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        if (errno != 0)
        {
            perror("server bind error");
            exit(EXIT_FAILURE);
        }
        DieWithSystemMessage("bind() failed");
    }
    fputs("server bind successfully\n", stdout);

    // fprintf可以用控制输出的格式控制符
    fprintf(stdout, "Server Bind to %lu port successfully\n", serverPort);

    // listen and get a new socket
    if (listen(serverSocket, MAXINCOME) == -1)
        DieWithSystemMessage("listen() failed");

    fprintf(stdout, "server listen at %lu\n", serverPort);

    // listen
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLen;

    errno = 0;
    int acceptSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, clientAddrLen);
    if (errno != 0)
    {
        perror("server accept error");
        exit(EXIT_FAILURE);
    }

    if (acceptSocket == -1)
        DieWithSystemMessage("accept() failed");

    // 这里尝试输出client的信息
    char clientName[INET_ADDRSTRLEN];
    if ((inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, clientName, sizeof(INET_ADDRSTRLEN))) == NULL)
        DieWithSystemMessage("inet_pton() failed");
    printf("%s/%d\n", clientName, ntohs(clientAddr.sin_port));

    // server recv
    char buf[BUFSIZ];
    ssize_t numByteRecv = recv(acceptSocket, buf, BUFSIZ - 1, 0);
    if (numByteRecv < 0)
        DieWithSystemMessage("recv() failed");
    buf[numByteRecv] = '\0';

    fputs("Server received message:", stdout);
    fputc('\n', stdout);
    fputs(buf, stdout);

    if (strcmp(buf, "GET\n") == 0)
        DieWithUserMessage("did'nt recv GET request");

    char *sendString = "HTTP/1.1 400 Bad Request\n";
    fputs("server Send message:", stdout);
    fputs("\n", stdout);
    fputs(sendString, stdout);

    // server send
    size_t sendStringLen = strlen(sendString) + 1;

    ssize_t numBytesent = send(acceptSocket, sendString, sendString, 0);
    if (numBytesent == -1)
        DieWithSystemMessage("send() failed");

    close(acceptSocket);

    close(serverSocket);

    return 0;
}