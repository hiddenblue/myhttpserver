#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "errorPrint.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "printDNSAddr.h"
// 这里忘了写包含被调用函数的头文件，但是被gcc纠正了

int main(int argc, char *argv[])
{
    if (argc != 3)
        DieWithUserMessage("Error Parameters(s)\n"
                           "<Servername> <Service/Port>");

    char *servername = argv[1];
    char *serverPort = argv[2];

    struct addrinfo addrHint;
    memset(&addrHint, 0, sizeof(addrHint));
    addrHint.ai_family = AF_UNSPEC;
    addrHint.ai_socktype = SOCK_STREAM;
    addrHint.ai_protocol = IPPROTO_TCP;

    struct addrinfo *addrList;

    int rtnVal = getaddrinfo(servername, serverPort, &addrHint, &addrList);
    if (rtnVal != 0)
        DieWithSystemMessage("getaaddrinfo() failed");

    for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next)
    {
        PrintSockaddr(addr->ai_addr, stdout);
        // 这里需要注意getddrinfo返回的addinfo类型的ai_addr和ai_next都是指针类型的数据
        fputc('\n', stdout);
    }

    // 这里不小心把freeaddrinfo写成了free() 链表会有内存泄漏
    freeaddrinfo(addrList);
    exit(0);
}
