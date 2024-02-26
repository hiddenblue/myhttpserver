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

    int serverSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    // new a sockaddr for server binding
    struct sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    // 这里ipv6的in6addr_any(小写）不需要转换成network byte order
    // 也不需要检查是否成功，因为是内置的宏
    // serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); v4版本
    serverAddr.sin6_addr = in6addr_any;
    serverAddr.sin6_port = htons(serverPort);

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
    fprintf(stdout, "Server Bind to %u port successfully\n", serverPort);

    // listen and get a new socket
    if (listen(serverSocket, MAXINCOME) == -1)
        DieWithSystemMessage("listen() failed");

    fprintf(stdout, "server listen at %u\n", serverPort);
    // listen

    // 后面是一个无线循环的accept，然后处理请求的过程
    for (;;)
    {
        struct sockaddr_in6 clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t clientAddrLen = sizeof(clientAddr);

        errno = 0;
        // 这里的addr_len必须是指针类型，否则会出错。
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (errno != 0)
        {
            perror("server accept error");
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "accept clinet income successfully\n");
        if (clientSocket == -1)
            DieWithSystemMessage("accept() failed");

        errno = 0;
        // 这里尝试输出client的信息
        char clientName[INET6_ADDRSTRLEN];
        // pton等函数如果可以执行AF FAMILY就说明不用对sockaddr进行类型的转换
        // 这里的长度传递出错了，不能传INET_ADDRSTRLEN 而必须是clntname长度
        if ((inet_ntop(AF_INET6, &clientAddr.sin6_addr, clientName, sizeof(clientName))) == NULL)
        {
            if (errno != 0)
                perror("inet_pton() error! ");
        }
        printf("%s / %d\n", clientName, ntohs(clientAddr.sin6_port));

        HandleTCPClient(clientSocket);
    }

    // NOT reach
    close(serverSocket);

    return 0;
}