
#include "printDNSAddr.h"

/**
 * @brief print sockaddr address and port. support both Ipv4 and Ipv6.
 * @param sockAddr
 * @param stream
 */
void PrintSockaddr(struct sockaddr *sockAddr, FILE *stream)
{
    if ((sockAddr == NULL) || stream == NULL)
        DieWithUserMessage("sockaddr or stream NULL pointer ");

    void *addrResult;

    // 需要把域名地址转换到数组中才能进行打印

    char buffer[INET6_ADDRSTRLEN];
    in_port_t port;

    // 针对ipv4 ipv6 和其他情况分别处理ip地址和端口
    switch (sockAddr->sa_family)
    {
    case AF_INET:
        addrResult = &((struct sockaddr_in *)sockAddr)->sin_addr;
        port = ntohs(((struct sockaddr_in *)sockAddr)->sin_port);
        break;

    case AF_INET6:
        addrResult = &((struct sockaddr_in6 *)sockAddr)->sin6_addr;
        port = ntohs(((struct sockaddr_in *)sockAddr)->sin_port);
        break;

    default:
        DieWithSystemMessage("error! unknown socket family type");
        break;
    }

    // ipv4和ipv6的字符使用同一个pton和ntop转换函数，支持参数进行调整

    if (inet_ntop(sockAddr->sa_family, addrResult, buffer, sizeof(buffer)) == 0)
        DieWithUserMessage("inet_ntop() failed");
    else
    {
        fprintf(stdout, "Address: %s\n", buffer);
        // 这里port不能等于0， 不是一个合法的端口
        if (port != 0)
            fprintf(stdout, "Port: %d\n", port);
    }
}