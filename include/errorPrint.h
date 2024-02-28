#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

#ifndef __ERRORPRINT_H
#define __ERRORPRINT_H

#define PRINTERROR(str) fprintf(stdout, "\033[31mFunction  %s() error at file %s line %d Info: %s\033[0m\n", __func__, __FILE__, __LINE__, str);

void DieWithUserMessage(const char *msg);

void DieWithSystemMessage(const char *msg);

void HandleTCPClient(int clientSocket);

int clientAppliction(int serversocket, const char *sendString);

void printRemoteSocket(int af, int socket);

void printLocalSocket(int __af, int socket);

#endif
