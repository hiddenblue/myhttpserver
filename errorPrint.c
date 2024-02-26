
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
    // server recv
    char buf[BUFSIZ];
    ssize_t numByteRecv = recv(clientSocket, buf, BUFSIZ - 1, 0);
    if (numByteRecv < 0)
        DieWithSystemMessage("recv() failed");
    buf[numByteRecv] = '\0';

    fputs("Server received message: ", stdout);
    fputs(buf, stdout);

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