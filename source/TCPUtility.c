#include "TCPUtility.h"

// funcction help echo client setup tcp connection to server v4 or v6
int setupTCPServerconn(const char *servername, const char *port)
{
    if (servername == NULL || port == NULL)
        DieWithUserMessage("servername or port NULL pointer error");

    struct addrinfo addrInfoHint;
    memset(&addrInfoHint, 0, sizeof(addrInfoHint));
    addrInfoHint.ai_family = AF_UNSPEC;
    addrInfoHint.ai_protocol = IPPROTO_TCP;
    addrInfoHint.ai_socktype = SOCK_STREAM;

    struct addrinfo *addrList;

    int rtnVal = getaddrinfo(servername, port, &addrInfoHint, &addrList);
    if (rtnVal != 0)
    {
        DieWithSystemMessage("getaddrinfo() failed");
        fputs(gai_strerror(rtnVal), stderr);
        fputc('\n', stderr);
    }

    int serverSocket = -1;
    for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next)
    {
        fputs("************************************************\n", stdout);
        PrintSockaddr(addr->ai_addr, stdout);

        serverSocket = socket(addr->ai_family, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket < 0)
        {
            fprintf(stderr, "create socket for server :%s %s error\n", servername, port);
            continue;
        }
        fprintf(stdout, "successfully create socket: %d for server %s \n", serverSocket, servername);

        errno = 0;
        if ((connect(serverSocket, (struct sockaddr *)addr->ai_addr, addr->ai_addrlen)) != 0)
        {

            if (errno != 0)
            {
                perror("client connect server failed");
                close(serverSocket);
                serverSocket = -1;
                continue;
            }
        }
        fprintf(stdout, "client successfully connect to %s\n", servername);
        break;
    }
    freeaddrinfo(addrList);
    return serverSocket;
}

int setupTCPListen(const char *serverPort)
{
    static const int MAXINCOME = 5;

    // make perrno work at the start of function
    errno = 0;

    fprintf(stdout, "start setupTCPClientConn() serverPort: %s\n", serverPort);

    if (serverPort == NULL)
        DieWithUserMessage("setupTCPClientConn() serverPort NULL pointer");

    struct addrinfo addrinfoHint;
    memset(&addrinfoHint, 0, sizeof(addrinfoHint));
    addrinfoHint.ai_family = AF_UNSPEC;
    // we accept both ipv4 and ipv6 address
    addrinfoHint.ai_flags = AI_PASSIVE;
    // combine this flags and set getaddrinfo first parm to NULL
    addrinfoHint.ai_protocol = IPPROTO_TCP;
    addrinfoHint.ai_socktype = SOCK_STREAM;

    struct addrinfo *AddrList;

    int rtnVal = getaddrinfo(NULL, serverPort, &addrinfoHint, &AddrList);
    if (rtnVal != 0 && errno != 0)
    {
        perror("getaddrinfo()");
        DieWithSystemMessage("getaddrinfo() failed");
    }

    int serverSocket = -1;
    for (struct addrinfo *addrInfo = AddrList; addrInfo != NULL; addrInfo = addrInfo->ai_next)
    {
        PrintSockaddr(addrInfo->ai_addr, stdout);

        fprintf(stdout, "start create socket, bind, listen\n");
        // 这里下面又不小心把serverSocket声明定义了，导致最后又变成1出错了
        serverSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
        if (serverSocket < 0)
        {
            fprintf(stdout, "host create socket failed. continue\n");
            perror("sockt()");
            continue;
        }
        fprintf(stdout, "creat socket: %d successfully\n", serverSocket);

        errno = 0;
        if (bind(serverSocket, (struct sockaddr *)addrInfo->ai_addr, sizeof(*addrInfo->ai_addr)) == 0 &&
            listen(serverSocket, MAXINCOME) == 0)
        {
            fprintf(stdout, "server bind and listen successfully\n");
            break;
        }
        else
        {
            close(serverSocket);
            serverSocket = -1;
            perror("bind() and listen()");
            continue;
        }
    }
    freeaddrinfo(AddrList);
    return serverSocket;
}

/**
 * @brief input an serverSocket return the clientSocket create by accept()
 * @param serverSocket
 * @return clientSocket
 */
int acceptTCPConn(int serverSocket)
{
    errno = 0;

    struct sockaddr_storage clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLen = sizeof(clientAddr);
    // 这里的addr_len必须是指针类型，否则会出错。
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

    // print socket local and peer infomation
    printLocalSocket(clientAddr.ss_family, clientSocket);
    printRemoteSocket(clientAddr.ss_family, clientSocket);

    if (errno != 0)
    {
        PRINTERROR(strerror(errno));
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "accept clinet income successfully\n");
    if (clientSocket == -1)
        DieWithSystemMessage("accept() failed");

    errno = 0;
    PrintSockaddr((struct sockaddr *)&clientAddr, stdout);
    if (errno != 0)
    {
        PRINTERROR("clientAddr error");
    }

    return clientSocket;
}
