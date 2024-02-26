
#include "errorPrint.h"

void DieWithUserMessage(const char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(-1);
}

void DieWithSystemMessage(const char *msg)
{
    fputs(msg, stderr);
    fputs("\n", stderr);
    exit(-1);
}

void HandleTCPClient(int clientSocket)
{
    // 这部分内容位于应用层，不涉及传输层和ip层，不需要关心ipv6还是ipv4
    // server recv
    char buf[BUFSIZ];
    ssize_t numByteRecv = recv(clientSocket, buf, BUFSIZ - 1, 0);
    if (numByteRecv < 0)
        DieWithSystemMessage("recv() failed");
    buf[numByteRecv] = '\0';

    fputs("Server received message: ", stdout);
    fputs(buf, stdout);
    fputc('\n', stdout);

    if (strcmp(buf, "GET\n") == 0)
        DieWithUserMessage("did'nt recv GET request");

    char *sendString = "HTTP/1.1 400 Bad Request\n";
    fputs("server Send message: ", stdout);
    fputs("\n", stdout);
    fputs(sendString, stdout);

    // server send
    size_t sendStringLen = strlen(sendString) + 1;

    ssize_t numBytesent = send(clientSocket, sendString, sendStringLen, 0);
    if (numBytesent == -1)
        DieWithSystemMessage("send() failed");

    fprintf(stdout, "\n");
    close(clientSocket);
}

// 暂时不考虑传回main函数的问题
int clientAppliction(int serverSocket, const char *sendstring)
{
    errno = 0;
    char buf[BUFSIZ];

    ssize_t numByteSent = 0;
    ssize_t echoStringLen = strlen(sendstring) + 1;
    numByteSent = send(serverSocket, sendstring, echoStringLen, 0);
    if (numByteSent == -1)
        DieWithSystemMessage("send() failed");
    else if (numByteSent < echoStringLen)
        DieWithSystemMessage("send() numBytesent < echoStringLen");

    ssize_t numByteRecv = 0;
    numByteRecv = recv(serverSocket, buf, BUFSIZ - 1, 0);

    if (numByteRecv == -1)
        DieWithSystemMessage("recv() faild numByteRecv == -1");
    buf[numByteRecv] = '\0';

    fputs("Received bytes are: ", stdout);
    fputs("\n", stdout);
    fputs(buf, stdout);
    fputs("\n", stdout);

    close(serverSocket);

    if (errno != 0)
    {
        perror("clientAppliction() error");
        return -1;
    }
    else
        return 0;
}