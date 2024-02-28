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

    int serverSocket = setupTCPClientConn(argv[1]);
    if (serverSocket < 0)
        DieWithUserMessage("setupTCPClientConn() failed");

    // 后面是一个无线循环的accept，然后处理请求的过程
    for (;;)
    {
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t clientAddrLen = sizeof(clientAddr);

        errno = 0;
        // 这里的addr_len必须是指针类型，否则会出错。
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

        // print socket local and peer infomation
        printLocalSocket(AF_INET, clientSocket);
        printRemoteSocket(AF_INET, clientSocket);

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
        char clientName[INET_ADDRSTRLEN];
        // pton等函数如果可以执行AF FAMILY就说明不用对sockaddr进行类型的转换
        // 这里的长度传递出错了，不能传INET_ADDRSTRLEN 而必须是clntname长度
        if ((inet_ntop(AF_INET, &clientAddr.sin_addr.s_addr, clientName, sizeof(clientName))) == NULL)
        {
            if (errno != 0)
                perror("inet_pton() error! ");
        }
        printf("%s / %d\n", clientName, ntohs(clientAddr.sin_port));

        HandleTCPClient(clientSocket);
    }

    // NOT reach
    close(serverSocket);

    return 0;
}